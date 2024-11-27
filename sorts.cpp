#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <tbb/tbb.h>

const int BLOCK_SIZE = 512;
const int RUNS = 5;
const int MAX_THREADS = 4;
const std::string YES = "\033[1;32mYES\033[0m";
const std::string NO = "\033[1;31mNO\033[0m";

int generate_random_number(int low, int high) {
    return low + rand() % (high - low + 1);
}

int partition(std::vector<int> &arr, int low, int high) {
    int element = arr[generate_random_number(low, high)];
    int i = low;
    int j = high;
    while (i <= j) {
        while (arr[i] < element) {
            ++i;
        }
        while (arr[j] > element) {
            --j;
        }
        if (i >= j) {
            break;
        }
        std::swap(arr[i++], arr[j--]);
    }
    return j;
}

void quicksort_seq(std::vector<int> &arr, int low, int high) {
    if (low < high) {
        int q = partition(arr, low, high);
        quicksort_seq(arr, low, q);
        quicksort_seq(arr, q + 1, high);
    }
}

void quicksort_par(std::vector<int> &arr, int low, int high) {
    if (low < high) {
        if (high - low < BLOCK_SIZE) {
            quicksort_seq(arr, low, high);
            return;
        }
        int q = partition(arr, low, high);
        tbb::parallel_invoke(
                [&] { quicksort_par(arr, low, q); },
                [&] { quicksort_par(arr, q + 1, high); }
        );
    }
}

bool is_sorted(const std::vector<int> &arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}

std::pair<bool, double> measure_sort_performance(
        const std::vector<int> &arr,
        const std::function<void(std::vector<int> &, int, int)> &sort_function
) {
    bool is_correct = true;
    double total_time = 0.0;
    for (int i = 0; i < RUNS; ++i) {
        auto arr_copy = arr;
        auto start = std::chrono::steady_clock::now();
        sort_function(arr_copy, 0, arr_copy.size() - 1);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        total_time += elapsed.count();
        is_correct &= is_sorted(arr_copy);
    }
    return {is_correct, total_time / RUNS};
}

std::vector<int> generate_random_array(size_t size) {
    std::vector<int> arr(size);
    for (int &num: arr) {
        num = generate_random_number(0, 1'000'000'000);
    }
    return arr;
}

int main() {
    std::srand(std::time(nullptr));

    tbb::task_arena arena(MAX_THREADS);

    auto arr_sizes = {1000, 100'000, 1'000'000, 10'000'000, 100'000'000};

    for (auto &arr_size: arr_sizes) {
        auto arr = generate_random_array(arr_size);

        std::cout << "Array size: " << arr_size << std::endl;

        auto [correct_seq_sort, seq_time] = measure_sort_performance(arr, quicksort_seq);
        std::cout << "Average sequential time: " << seq_time << " seconds" << std::endl;
        std::cout << "Sequential sort correct: " << (correct_seq_sort ? YES : NO) << std::endl;

        auto [correct_par_sort, par_time] = measure_sort_performance(
                arr,
                [&](std::vector<int> &arr, int low, int high) {
                    arena.execute([&] {
                        quicksort_par(arr, low, high);
                    });
                }
        );
        std::cout << "Average parallel time: " << par_time << " seconds" << std::endl;
        std::cout << "Parallel sort correct: " << (correct_par_sort ? YES : NO) << std::endl;

        std::cout << "Boost: " << seq_time / par_time << std::endl << std::endl;
    }

    return 0;
}