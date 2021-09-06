# Performance Log of DeepRTS
| Version | Date | UPS | maxfps | Reason |
|-------|----------|-------------|----------|------|
|Early game version| 27.01.17| x25 500| 1| Fewer game logics|
|Pure python|03.02.17|x13 000| 1 | First "full" version|
|100% Cython| 13.02.17| x17 000| 1| Converted all files to Cython, nothing more|
|Pure python| 14.02.17| x20 000| 1| Went back to python because of suboptimal results
|Dict impl| 14.02.17 | x25 000 | 1 | Improved game engine by using dict as data container for Player/Unit. Faster load/save
|Player.pyx| 14.02.17 | x37 000 | 1 | Static Typing on all fields (Classes defined as object)
|Game.pyx | 14.02.17 | x45 000 | 1 | Static Typing on all fields (Classes defined as object)
|States| 15.02.17 | x55 000 | 1 | Static Typing on all states
|Units| 15.02.17 | x77 000 | 1 | Static Typing on all units
|Partial Conversion | 16.02.17 | x180 000 | 1 | Most code rewritten to Cython. Still much optimization potential
|Cython 1.0 | 24.02.17 | x500 000 | 1 | Full Cython stack. However, all stacktrace gone
|C++ | 27.02.17 | x2 700 000 | 60 | Early version of the game in C++
|C++ | 10.03.17 | x8 500 000 | 60 | Version 1.0 (Heap)
|C++ | 13.03.17 | x52 000 000 | 60 | Version 1.1 (Stack) (NoGui) (Simple Engine)
|C++ | 06.09.21 | x6 500 000 | x6 500 000 | Version 3.0 (Stack) (NoGui) (Full Engine))
|C++ | 06.09.21 | x3 000 000 | x3 000 000 | Version 3.0 (Stack) (GUI) (Full Engine))
These tests were done on 2x Intel(R) Xeon(R) CPU X5660 @ 2.80GHz where tests ran on a single core
