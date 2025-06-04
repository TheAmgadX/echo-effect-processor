# 🎧 Echo Effect Processor

Hello everyone! This is my latest project — **Echo Effect Processor**.
It takes a `.wav` audio file, adds an echo effect to it, and outputs a new `.wav` file.

The cool part? The project uses **SIMD (Single Instruction, Multiple Data)** for performance optimization. You can find a **performance comparison plot** in the `scripts` directory that shows a **huge improvement** between the naive and SIMD approaches.

---

## 🧠 Why I Built This Project

This project started as a requirement for my **Computer Architecture** course.
We were asked to build any application and apply optimization techniques that we learned during the course.

I chose **audio processing** — and honestly, I didn’t know why at first. I just wanted to explore something new. I had **zero experience** in audio processing, didn’t even know how audio is represented in computers.

I started from scratch:

* Learned the basics of digital audio
* Figured out how to read and write `.wav` files in C++
* Researched how to apply an echo effect

It was a challenge, but I loved it.

After getting the naive version working, I moved on to optimizing it using SIMD — and the performance boost was really satisfying.

---

## 🛠️ Project Structure

```
.
├── CMakeLists.txt
├── include
│   ├── AudioFile.h
│   ├── echo_processor.h
│   └── Visuals.h
├── README.md
├── scripts
│   ├── generate_plots.py
│   └── performance_comparison.png
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

Check out the `scripts/performance_comparison.png` to see how much faster the SIMD version is compared to the naive implementation. You can also run the `generate_plots.py` script to regenerate the plot.

---

## Challenges I Faced

* Understanding how audio data is structured in `.wav` files
* Reading and writing audio data in C++
* Implementing the echo effect correctly
* Optimizing with SIMD without breaking correctness
* Structuring the project and writing clean, reusable code

---

## 💬 Final Thoughts

This project pushed me out of my comfort zone — and that’s exactly why I enjoyed it.
Learning new things, debugging tricky parts, and seeing the performance improvements was super rewarding.

Thanks for checking it out!