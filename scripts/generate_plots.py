import matplotlib.pyplot as plt

# File sizes in MB for x-axis
file_sizes_bytes = [341566, 8303192, 63002768, 433988848, 2145246216]
file_sizes_mb = [size / (1024 * 1024) for size in file_sizes_bytes]

# Updated execution times from your recent runs
naive_times = [0.0126289, 0.126014, 0.935011, 7.16891, 33.5623]
loop_unrolled_times = [0.00585077, 0.131101, 0.95755, 6.27482, 30.7633]
simd_times = [0.00288648, 0.0691625, 0.5262, 3.57969, 18.158]

# Plotting
plt.figure(figsize=(10, 6))

plt.plot(file_sizes_mb, naive_times, marker='o', label='Naive')
plt.plot(file_sizes_mb, loop_unrolled_times, marker='s', label='Loop Unrolled')

# For SIMD, only plot points with valid data
valid_simd_indices = [i for i, t in enumerate(simd_times) if t is not None]
valid_simd_sizes = [file_sizes_mb[i] for i in valid_simd_indices]
valid_simd_times = [simd_times[i] for i in valid_simd_indices]

plt.plot(valid_simd_sizes, valid_simd_times, marker='^', label='SIMD')

plt.title('Performance Comparison of Audio Processing Approaches')
plt.xlabel('File Size (MB)')
plt.ylabel('Execution Time (s)')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('performance_comparison.png')
# plt.show()

# Optional: print totals ignoring missing SIMD values
total_naive = sum(naive_times)
total_loop_unrolled = sum(loop_unrolled_times)
total_simd = sum(t for t in simd_times if t is not None)

(total_naive, total_loop_unrolled, total_simd)
