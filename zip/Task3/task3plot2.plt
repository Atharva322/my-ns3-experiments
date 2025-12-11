set title "TCP Congestion Window - Scenario 2"
set xlabel "Time (s)"
set ylabel "Congestion Window (KB)"
set terminal pngcairo size 800,600
set output "prasadma-2.png"
plot "prasadma-2.dat" using 1:2 title "CWND" with lines lw 2 linecolor rgb "red"
