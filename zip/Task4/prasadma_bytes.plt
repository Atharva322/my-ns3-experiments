# prasadma_flows.plt - Gnuplot script to plot Flow 1 and Flow 2 data
set terminal pngcairo size 800,600 enhanced font "Arial,10"
set output "prasadma_flows.png"

set title "Cumulative Bytes Received vs Time for Two UDP Flows"
set xlabel "Time (s)"
set ylabel "Cumulative Bytes Received"
set xrange [0:10]
set yrange [0:*]
set key top left
set grid ytics lc rgb "#cccccc" lt 1

plot "prasadma_flow1.dat" using 1:2 with linespoints lt 1 lw 2 pt 7 \
     title "Flow 1 (A->B)", \
     "prasadma_flow2.dat" using 1:2 with lines lt 2 lw 2 \
     title "Flow 2 (A->D)"
