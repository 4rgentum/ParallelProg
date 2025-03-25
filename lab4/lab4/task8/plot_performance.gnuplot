# Set output format
set terminal pngcairo size 1200, 800 enhanced font "Arial, 14"
set style data linespoints

# Define input files and corresponding labels
files = "performance_static_chunk_5.txt performance_static_chunk_10.txt performance_static_chunk_25.txt performance_static_chunk_50.txt performance_dynamic_chunk_5.txt performance_dynamic_chunk_10.txt performance_dynamic_chunk_25.txt performance_dynamic_chunk_50.txt performance_guided_chunk_5.txt performance_guided_chunk_10.txt performance_guided_chunk_25.txt performance_guided_chunk_50.txt"
labels = "Static_Chunk_5 Static_Chunk_10 Static_Chunk_25 Static_Chunk_50 Dynamic_Chunk_5 Dynamic_Chunk_10 Dynamic_Chunk_25 Dynamic_Chunk_50 Guided_Chunk_5 Guided_Chunk_10 Guided_Chunk_25 Guided_Chunk_50"

# Plot Avg Time vs Threads
set output "avg_time_vs_threads.png"
set title "Average Time vs Number of Threads"
set xlabel "Number of Threads"
set ylabel "Average Time (s)"
set grid
plot for [i=1:words(files)] word(files, i) using 1:2 title word(labels, i) lc i

# Plot Speedup vs Threads
set output "speedup_vs_threads.png"
set title "Speedup vs Number of Threads"
set xlabel "Number of Threads"
set ylabel "Speedup"
set grid
plot for [i=1:words(files)] word(files, i) using 1:3 title word(labels, i) lc i

# Plot Efficiency vs Threads
set output "efficiency_vs_threads.png"
set title "Efficiency vs Number of Threads"
set xlabel "Number of Threads"
set ylabel "Efficiency"
set grid
plot for [i=1:words(files)] word(files, i) using 1:4 title word(labels, i) lc i
