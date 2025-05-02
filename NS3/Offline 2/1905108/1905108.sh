#!/bin/bash
mkdir "scratch/1905108"
echo "Executing 1905108.cc for diffrent graphs..."

for((i=1;i<100;i=i+10))
do
echo for ${i}
./ns3 run "scratch/1905108.cc -bDatarate=${i} -tcp2=ns3::TcpAdaptiveReno -graphtype=1"
done


//gnuplot
gnuplot <<- EOF
        set xlabel "Bottleneck Data rate"
        set ylabel "Throughput"
        set title "ThroughPut vs Bottleneck Data rate"   
        set terminal png size 800,600
        set output "scratch/1905108/th_vs_bdata.png"
        plot "scratch/throughput1.dat" using 1:2 title 'TCP new reno' with linespoints,\
        "scratch/throughput2.dat" using 1:2 title 'TCP adaptive reno' with linespoints
        set xlabel "Bottleneck Data rate"
        set ylabel "Jain Index"
        set title "Jain Index vs Bottleneck Data rate"   
        set terminal png size 800,600
        set output "scratch/1905108/jain_vs_bdata.png"
        plot "scratch/jainfair.dat" using 1:2 title 'Jain Index' with linespoints
        exit
EOF

echo "" > scratch/throughput1.dat
echo "" > scratch/throughput2.dat
echo "" > scratch/jainfair.dat
echo ""


echo "Executing 1905108.cc for diffrent pktLossExp..."


for((i=2;i<7;i=i+1))
do
echo for ${i}
./ns3 run "scratch/1905108.cc -pktLossExp=${i} -graphtype=2"
done

gnuplot <<- EOF
        set xlabel "Packet Loss rate"
        set ylabel "Throughput"
        set title "ThroughPut vs Packet Loss rate"   
        set terminal png size 800,600
        set output "scratch/1905108/th_vs_prate.png"
        plot "scratch/throughput1.dat" using 1:2 title 'TCP new reno' with linespoints,\
        "scratch/throughput2.dat" using 1:2 title 'TCP adaptive reno' with linespoints
        set xlabel "Packet Loss rate"
        set ylabel "Jain Index"
        set title "Jain Index vs Packet Loss rate"   
        set terminal png size 800,600
        set output "scratch/1905108/jain_vs_prate.png"
        plot "scratch/jainfair.dat" using 1:2 title 'Jain Index' with linespoints
        exit
EOF

echo "" > scratch/throughput1.dat
echo "" > scratch/throughput2.dat
echo "" > scratch/jainfair.dat
echo "" > scratch/confile1.dat
echo "" > scratch/confile2.dat
echo ""

echo "Executing 1905108.cc for congestion window..."
./ns3 run "scratch/1905108.cc -graphtype=3"

gnuplot <<- EOF
        set xlabel "Time"
        set ylabel "Congestion Window"
        set title "Congestion Window vs Time"   
        set terminal png size 800,600
        set output "scratch/1905108/cg_vs_time.png"
        plot "scratch/confile1.dat" using 1:2 title 'TCP new reno' with linespoints,\
        "scratch/confile2.dat" using 1:2 title 'TCP adaptive reno' with linespoints
        exit
EOF


echo "" > scratch/throughput1.dat
echo "" > scratch/throughput2.dat
echo "" > scratch/jainfair.dat
echo "" > scratch/confile1.dat
echo "" > scratch/confile2.dat
echo ""

for((i=1;i<100;i=i+10))
do
echo for ${i}
./ns3 run "scratch/1905108.cc -bDatarate=${i} -tcp2=ns3::TcpHighSpeed -graphtype=1"
done


//gnuplot
gnuplot <<- EOF
        set xlabel "Bottleneck Data rate"
        set ylabel "Throughput"
        set title "ThroughPut vs Bottleneck Data rate"   
        set terminal png size 800,600
        set output "scratch/1905108/th_vs_bdata(high).png"
        plot "scratch/throughput1.dat" using 1:2 title 'TCP new reno' with linespoints,\
        "scratch/throughput2.dat" using 1:2 title 'TCP high speed' with linespoints
        set xlabel "Bottleneck Data rate"
        set ylabel "Jain Index"
        set title "Jain Index vs Bottleneck Data rate"   
        set terminal png size 800,600
        set output "scratch/1905108/jain_vs_bdata(high).png"
        plot "scratch/jainfair.dat" using 1:2 title 'Jain Index' with linespoints
        exit
EOF

echo "" > scratch/throughput1.dat
echo "" > scratch/throughput2.dat
echo "" > scratch/jainfair.dat
echo ""


echo "Executing 1905108.cc for diffrent pktLossExp..."


for((i=2;i<7;i=i+1))
do
echo for ${i}
./ns3 run "scratch/1905108.cc -pktLossExp=${i} -tcp2=ns3::TcpHighSpeed -graphtype=2"
done

gnuplot <<- EOF
        set xlabel "Packet Loss rate"
        set ylabel "Throughput"
        set title "ThroughPut vs Packet Loss rate"   
        set terminal png size 800,600
        set output "scratch/1905108/th_vs_prate(high).png"
        plot "scratch/throughput1.dat" using 1:2 title 'TCP new reno' with linespoints,\
        "scratch/throughput2.dat" using 1:2 title 'TCP high speed' with linespoints
        set xlabel "Packet Loss rate"
        set ylabel "Jain Index"
        set title "Jain Index vs Packet Loss rate"   
        set terminal png size 800,600
        set output "scratch/1905108/jain_vs_prate(high).png"
        plot "scratch/jainfair.dat" using 1:2 title 'Jain Index' with linespoints
        exit
EOF

echo "" > scratch/throughput1.dat
echo "" > scratch/throughput2.dat
echo "" > scratch/jainfair.dat
echo "" > scratch/confile1.dat
echo "" > scratch/confile2.dat
echo ""

echo "Executing 1905108.cc for congestion window..."
./ns3 run "scratch/1905108.cc -tcp2=ns3::TcpHighSpeed -graphtype=3"

gnuplot <<- EOF
        set xlabel "Time"
        set ylabel "Congestion Window"
        set title "Congestion Window vs Time"   
        set terminal png size 800,600
        set output "scratch/1905108/cg_vs_time(high).png"
        plot "scratch/confile1.dat" using 1:2 title 'TCP new reno' with linespoints,\
        "scratch/confile2.dat" using 1:2 title 'TCP High Speed' with linespoints
        exit
EOF