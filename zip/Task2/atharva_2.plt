set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'atharva_hist.png'
set title "Sent vs Received Packets"
set xlabel "Number of Packets Sent"
set ylabel "Number of Packets Received"
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.5
set grid ytics
plot "atharva_2.txt" using 2:xtic(1) title "Packets Received"

