set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'atharva_plot.png'
set title "Throughput vs Number of CSMA Nodes"
set xlabel "Number of CSMA Nodes"
set ylabel "Throughput (bps)"
set grid
plot "atharva_1.txt" using 1:2 with linespoints title "Throughput"

