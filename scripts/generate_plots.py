import matplotlib.pyplot as plt

# File sizes in MB for x-axis
file_sizes_bytes = [341566, 8303192, 63002768, 433988848, 2145246216]
file_sizes_mb = [size / (1024 * 1024) for size in file_sizes_bytes]

naive_times = [0.0126289, 0.126014, 0.935011, 7.16891, 33.5623]
loop_unrolled_times = [0.00585077, 0.131101, 0.95755, 6.27482, 30.7633]
simd_times = [0.00291834, 0.0688519, 0.523488, 3.63386, 18.4324]
simd_threads_times = [0.0041629, 0.0711854, 0.414199, 2.79109, 14.1682]

# Create the plot
plt.figure(figsize=(12, 8))

# Plot all four approaches
plt.plot(file_sizes_mb, naive_times, marker='o', linewidth=2, markersize=8, label='Naive')
plt.plot(file_sizes_mb, loop_unrolled_times, marker='s', linewidth=2, markersize=8, label='Loop Unrolled')
plt.plot(file_sizes_mb, simd_times, marker='^', linewidth=2, markersize=8, label='SIMD')
plt.plot(file_sizes_mb, simd_threads_times, marker='D', linewidth=2, markersize=8, label='SIMD + Multithreading')

# Customize the plot
plt.title('Performance Comparison of Audio Processing Approaches', fontsize=16, fontweight='bold')
plt.xlabel('File Size (MB)', fontsize=14)
plt.ylabel('Execution Time (s)', fontsize=14)
plt.legend(fontsize=12)
plt.grid(True, alpha=0.3)
plt.yscale('log')  # Use log scale for better visualization of the wide range of values
plt.xscale('log')  # Use log scale for file sizes as well

# Add some styling
plt.tight_layout()

# Save the plot
plt.savefig('performance_comparison_all_approaches.png', dpi=300, bbox_inches='tight')
plt.show()

# Print summary statistics
print("=== Performance Summary ===")
print(f"Total execution times:")
print(f"Naive Approach: {sum(naive_times):.4f} s")
print(f"Loop Unrolled Approach: {sum(loop_unrolled_times):.4f} s") 
print(f"SIMD Approach: {sum(simd_times):.4f} s")
print(f"SIMD + Multithreading Approach: {sum(simd_threads_times):.4f} s")

print(f"\nSpeedup compared to Naive:")
print(f"Loop Unrolled: {sum(naive_times)/sum(loop_unrolled_times):.2f}x")
print(f"SIMD: {sum(naive_times)/sum(simd_times):.2f}x")
print(f"SIMD + Multithreading: {sum(naive_times)/sum(simd_threads_times):.2f}x")

# Create a separate bar chart for total execution times
plt.figure(figsize=(10, 6))
approaches = ['Naive', 'Loop Unrolled', 'SIMD', 'SIMD + Multithreading']
total_times = [sum(naive_times), sum(loop_unrolled_times), sum(simd_times), sum(simd_threads_times)]

bars = plt.bar(approaches, total_times, color=['#ff7f0e', '#2ca02c', '#d62728', '#9467bd'])
plt.title('Total Execution Time Comparison', fontsize=16, fontweight='bold')
plt.ylabel('Total Execution Time (s)', fontsize=14)
plt.yscale('log')

# Add value labels on bars
for bar, time in zip(bars, total_times):
    plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + bar.get_height()*0.1, 
            f'{time:.3f}s', ha='center', va='bottom', fontweight='bold')

plt.tight_layout()
plt.savefig('total_execution_time_comparison.png', dpi=300, bbox_inches='tight')
plt.show()