### Команда для запуска:

```bash
cmake -B build && cmake --build build && ./build/parallel_quicksort
```

### Результат исполнения: 

Array size: 1000\
Average sequential time: 0.000171417 seconds\
Sequential sort correct: <span style="color:green">YES</span>\
Average parallel time: 0.000145583 seconds\
Parallel sort correct: <span style="color:green">YES</span>\
Boost: 1.17745

Array size: 100000\
Average sequential time: 0.0180326 seconds\
Sequential sort correct: <span style="color:green">YES</span>\
Average parallel time: 0.00495146 seconds\
Parallel sort correct: <span style="color:green">YES</span>\
Boost: 3.64188

Array size: 1000000\
Average sequential time: 0.172022 seconds\
Sequential sort correct: <span style="color:green">YES</span>\
Average parallel time: 0.0556577 seconds\
Parallel sort correct: <span style="color:green">YES</span>\
Boost: 3.09071

Array size: 10000000\
Average sequential time: 1.95808 seconds\
Sequential sort correct: <span style="color:green">YES</span>\
Average parallel time: 0.599493 seconds\
Parallel sort correct: <span style="color:green">YES</span>\
Boost: 3.26623

Array size: 100000000\
Average sequential time: 21.6938 seconds\
Sequential sort correct: <span style="color:green">YES</span>\
Average parallel time: 6.59753 seconds\
Parallel sort correct: <span style="color:green">YES</span>\
Boost: 3.28818
