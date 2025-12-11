set terminal pngcairo size 800,600
set output 'cwnd_plot.png'
set title "TCP Congestion Window over Time"
set xlabel "Time (s)"
set ylabel "Congestion Window (KB)"
plot "prasadma.dat" using 1:2 with lines title "CWND"
