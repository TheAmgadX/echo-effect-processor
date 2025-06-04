# 🎧 Echo Effect Processor

Hello everyone! This is my latest project — **Echo Effect Processor**.
It takes a `.wav` audio file, adds an echo effect to it, and outputs a new `.wav` file.

The cool part? The project uses **SIMD (Single Instruction, Multiple Data) and multithreading** for performance optimization. You can find a **performance comparison plots** in the `scripts` directory that shows a **huge improvement** between the naive and SIMD approaches.

---

## 🧠 Why I Built This Project

This project started as a requirement for my **Computer Architecture** course.
We were asked to build any application and apply optimization techniques that we learned during the course.

I chose **audio processing** — and honestly, I didn’t know why at first. I just wanted to explore something new. I had **zero experience** in audio processing, didn’t even know how audio is represented in computers.

I started from scratch:

* Learned the basics of digital audio
* Learned how to read and write `.wav` files in C++
* Researched how to apply an echo effect

It was a challenge, but I enjoyed it.

After getting the naive version working, I moved on to optimizing it using SIMD — and the performance boost was really satisfying.

---

## 🛠️ Project Structure

```
.
├── CMakeLists.txt
├── include
│   ├── AudioFile.h
│   ├── echo_processor.h
│   └── Visuals.h
├── README.md
├── scripts
│   ├── generate_plots.py
│   ├── performance_comparison_all_approaches.png
│   └── total_execution_time_comparison.png
└── src
    ├── AudioFile.cpp
    ├── echo_naive.cpp
    ├── echo_processor.cpp
    ├── echo_simd_opt.cpp
    ├── main.cpp
    └── Visuals.cpp
```

---

## 🚀 How to Run It

You need **C++** and **CMake** installed.

1. Clone the repository:

   ```bash
   git clone https://github.com/TheAmgadX/echo-effect-processor.git
   cd echo-effect-processor
   ```
2. Create a build directory and compile:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
3. The executables will be in the `bin` directory.

---

## 📈 Performance Comparison

Check out the `scripts/` to see how much faster the SIMD version is compared to the naive implementation. You can also run the `generate_plots.py` script to regenerate the plots.

## Execution Times by File Size

| File Size (MB) | Naive (s) | Loop Unrolled (s) | SIMD (s) | SIMD + Threads (s) |
|----------------|-----------|-------------------|----------|--------------------|
| 0.33           | 0.0126    | 0.0059           | 0.0029   | 0.0042            |
| 7.92           | 0.1260    | 0.1311           | 0.0689   | 0.0712            |
| 60.07          | 0.9350    | 0.9576           | 0.5235   | 0.4142            |
| 413.98         | 7.1689    | 6.2748           | 3.6339   | 2.7911            |
| 2,046.07       | 33.5623   | 30.7633          | 18.4324  | 14.1682           |

## Total Performance Summary

| Approach | Total Time (s) | Speedup vs Naive |
|----------|----------------|------------------|
| Naive | 41.79 | 1.00x |
| Loop Unrolled | 37.99 | 1.10x |
| SIMD | 22.66 | 1.84x |
| SIMD + Multithreading | 17.52 | 2.38x |

## Speedup Comparison by File Size

| File Size (MB) | Loop Unrolled vs Naive | SIMD vs Naive | SIMD + Threads vs Naive |
|----------------|------------------------|---------------|-------------------------|
| 0.33           | 2.14x                 | 4.34x         | 3.00x                   |
| 7.92           | 0.96x                 | 1.83x         | 1.77x                   |
| 60.07          | 0.98x                 | 1.79x         | 2.26x                   |
| 413.98         | 1.14x                 | 1.97x         | 2.57x                   |
| 2,046.07       | 1.09x                 | 1.82x         | 2.37x                   |

---

## Challenges I Faced

* Understanding how audio data is structured in `.wav` files
* Reading and writing audio data in C++
* Implementing the echo effect correctly
* Optimizing with SIMD without breaking correctness

---

## 💬 Final Thoughts

This project pushed me out of my comfort zone — and that’s exactly why I enjoyed it.
Learning new things, debugging tricky parts, and seeing the performance improvements was super rewarding.

Thanks for checking it out!
