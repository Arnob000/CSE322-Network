# #!/bin/bash
mkdir "scratch/1905108"
echo "Executing 1905108_1.cc for diffrent node values..."

./ns3 run "scratch/1905108_1.cc -nWifi=6 -graphType=1"
./ns3 run "scratch/1905108_1.cc -nWifi=12 -graphType=1"
./ns3 run "scratch/1905108_1.cc -nWifi=18 -graphType=1"
./ns3 run "scratch/1905108_1.cc -nWifi=24 -graphType=1"
./ns3 run "scratch/1905108_1.cc -nWifi=30 -graphType=1"


//gnuplot
gnuplot <<- EOF
        set xlabel "Nodes"
        set ylabel "Delivery ratio"
        set title "Nodes Vs Delivery Ratio"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_1(nodes-delivery).png"
        plot "scratch/1905108/1905108_1delivery.dat" using 1:1 title 'Nodes VS  Delivery Ratio' with linespoints
        set xlabel "Nodes"
        set ylabel "ThroughPut"
        set title "Nodes Vs ThroughPut"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_1(nodes-throughput).png"
        plot "scratch/1905108/1905108_1throughput.dat" using 1:1 title 'Nodes VS ThroughPut' with linespoints     
        exit
EOF

echo "" > scratch/1905108/1905108_1throughput.dat
echo "" > scratch/1905108/1905108_1delivery.dat
echo ""


echo "Executing 1905108_1.cc for diffrent flow values..."
./ns3 run "scratch/1905108_1.cc -nFlow=10 -graphType=2"
./ns3 run "scratch/1905108_1.cc -nFlow=20 -graphType=2"
./ns3 run "scratch/1905108_1.cc -nFlow=30 -graphType=2"
./ns3 run "scratch/1905108_1.cc -nFlow=40 -graphType=2"
./ns3 run "scratch/1905108_1.cc -nFlow=50 -graphType=2"

gnuplot <<- EOF
        set xlabel "Flows"
        set ylabel "Delivery ratio"
        set title "Flows Vs Delivery Ratio"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_1(flow-delivery).png"
        plot "scratch/1905108/1905108_1delivery.dat" using 1:2 title 'Flows VS  Delivery Ratio' with linespoints
        set xlabel "Flows"
        set ylabel "ThroughPut"
        set title "Flows Vs ThroughPut"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_1(flow-throughput).png"
        plot "scratch/1905108/1905108_1throughput.dat" using 1:1 title 'Flows VS ThroughPut' with linespoints     
        exit
EOF


echo "" > scratch/1905108/1905108_1throughput.dat
echo "" > scratch/1905108/1905108_1delivery.dat
echo ""

echo "Executing 1905108_1.cc for diffrent number of packets per second..."
./ns3 run "scratch/1905108_1.cc -pcktRate=100 -graphType=3"
./ns3 run "scratch/1905108_1.cc -pcktRate=200 -graphType=3"
./ns3 run "scratch/1905108_1.cc -pcktRate=300 -graphType=3"
./ns3 run "scratch/1905108_1.cc -pcktRate=400 -graphType=3"
./ns3 run "scratch/1905108_1.cc -pcktRate=500 -graphType=3"

gnuplot <<- EOF
        set xlabel "Packet Rate"
        set ylabel "Delivery ratio"
        set title "Packet Rate Vs Delivery Ratio"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_1(pcktRate-delivery).png"
        plot "scratch/1905108/1905108_1delivery.dat" using 1:2 title 'Packet Rate VS  Delivery Ratio' with linespoints
        set xlabel "Packet Rate"
        set ylabel "ThroughPut"
        set title "Packet Rate Vs ThroughPut"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_1(pcktRate-throughput).png"
        plot "scratch/1905108/1905108_1throughput.dat" using 1:1 title 'Packet Rate VS ThroughPut' with linespoints     
        exit
EOF


echo "" > scratch/1905108/1905108_1throughput.dat
echo "" > scratch/1905108/1905108_1delivery.dat
echo ""

echo "Executing 1905108_1.cc for diffrent coverage area..."
./ns3 run "scratch/1905108_1.cc -coverageMultiplier=1 -graphType=4"
./ns3 run "scratch/1905108_1.cc -coverageMultiplier=2 -graphType=4"
./ns3 run "scratch/1905108_1.cc -coverageMultiplier=3 -graphType=4"
./ns3 run "scratch/1905108_1.cc -coverageMultiplier=4 -graphType=4"
./ns3 run "scratch/1905108_1.cc -coverageMultiplier=5 -graphType=4"

gnuplot <<- EOF
        set xlabel "Coverage"
        set ylabel "Delivery ratio"
        set title "Coverage Vs Delivery Ratio"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_1(coverage-delivery).png"
        plot "scratch/1905108/1905108_1delivery.dat" using 1:2 title 'Coverage VS  Delivery Ratio' with linespoints
        set xlabel "Coverage"
        set ylabel "ThroughPut"
        set title "Coverage Vs ThroughPut"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_1(coverage-throughput).png"
        plot "scratch/1905108/1905108_1throughput.dat" using 1:1 title 'Coverage VS ThroughPut' with linespoints     
        exit
EOF


# mobile

echo "Executing 1905108_2.cc for diffrent node values..."

./ns3 run "scratch/1905108_2.cc -nWifi=6 -graphType=1"
./ns3 run "scratch/1905108_2.cc -nWifi=12 -graphType=1"
./ns3 run "scratch/1905108_2.cc -nWifi=18 -graphType=1"
./ns3 run "scratch/1905108_2.cc -nWifi=24 -graphType=1"
./ns3 run "scratch/1905108_2.cc -nWifi=30 -graphType=1"


//gnuplot
gnuplot <<- EOF
        set xlabel "Nodes"
        set ylabel "Delivery ratio"
        set title "Nodes Vs Delivery Ratio"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_2(nodes-delivery).png"
        plot "scratch/1905108/1905108_2delivery.dat" using 1:1 title 'Nodes VS  Delivery Ratio' with linespoints
        set xlabel "Nodes"
        set ylabel "ThroughPut"
        set title "Nodes Vs ThroughPut"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_2(nodes-throughput).png"
        plot "scratch/1905108/1905108_2throughput.dat" using 1:1 title 'Nodes VS ThroughPut' with linespoints     
        exit
EOF


echo "" > scratch/1905108/1905108_2throughput.dat
echo "" > scratch/1905108/1905108_2delivery.dat
echo ""


echo "Executing 1905108_2.cc for diffrent flow values..."
./ns3 run "scratch/1905108_2.cc -nFlow=10 -graphType=2"
./ns3 run "scratch/1905108_2.cc -nFlow=20 -graphType=2"
./ns3 run "scratch/1905108_2.cc -nFlow=30 -graphType=2"
./ns3 run "scratch/1905108_2.cc -nFlow=40 -graphType=2"
./ns3 run "scratch/1905108_2.cc -nFlow=50 -graphType=2"

gnuplot <<- EOF
        set xlabel "Flows"
        set ylabel "Delivery ratio"
        set title "Flows Vs Delivery Ratio"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_2(flow-delivery).png"
        plot "scratch/1905108/1905108_2delivery.dat" using 1:2 title 'Flows VS  Delivery Ratio' with linespoints
        set xlabel "Flows"
        set ylabel "ThroughPut"
        set title "Flows Vs ThroughPut"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_2(flow-throughput).png"
        plot "scratch/1905108/1905108_2throughput.dat" using 1:1 title 'Flows VS ThroughPut' with linespoints     
        exit
EOF


echo "" > scratch/1905108/1905108_2throughput.dat
echo "" > scratch/1905108/1905108_2delivery.dat
echo ""

echo "Executing 1905108_2.cc for diffrent packet per second..."
./ns3 run "scratch/1905108_2.cc -pcktRate=100 -graphType=3"
./ns3 run "scratch/1905108_2.cc -pcktRate=200 -graphType=3"
./ns3 run "scratch/1905108_2.cc -pcktRate=300 -graphType=3"
./ns3 run "scratch/1905108_2.cc -pcktRate=400 -graphType=3"
./ns3 run "scratch/1905108_2.cc -pcktRate=500 -graphType=3"

gnuplot <<- EOF
        set xlabel "Packet Rate"
        set ylabel "Delivery ratio"
        set title "Packet Rate Vs Delivery Ratio"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_2(pcktRate-delivery).png"
        plot "scratch/1905108/1905108_2delivery.dat" using 1:2 title 'Packet Rate VS  Delivery Ratio' with linespoints
        set xlabel "Packet Rate"
        set ylabel "ThroughPut"
        set title "Packet Rate Vs ThroughPut"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_2(pcktRate-throughput).png"
        plot "scratch/1905108/1905108_2throughput.dat" using 1:1 title 'Packet Rate VS ThroughPut' with linespoints     
        exit
EOF



echo "" > scratch/1905108/1905108_2throughput.dat
echo "" > scratch/1905108/1905108_2delivery.dat
echo ""

echo "Executing 1905108_2.cc for diffrent moobility speed..."
./ns3 run "scratch/1905108_2.cc -speed=5 -graphType=4"
./ns3 run "scratch/1905108_2.cc -speed=10 -graphType=4"
./ns3 run "scratch/1905108_2.cc -speed=15 -graphType=4"
./ns3 run "scratch/1905108_2.cc -speed=20 -graphType=4"
./ns3 run "scratch/1905108_2.cc -speed=25 -graphType=4"

gnuplot <<- EOF
        set xlabel "Speed"
        set ylabel "Delivery ratio"
        set title "Speed Vs Delivery Ratio"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_2(speed-delivery).png"
        plot "scratch/1905108/1905108_2delivery.dat" using 1:1 title 'Speed VS  Delivery Ratio' with linespoints
        set xlabel "Speed"
        set ylabel "ThroughPut"
        set title "Speed Vs ThroughPut"   
        set terminal png size 800,600
        set output "scratch/1905108/1905108_2(speed-throughput).png"
        plot "scratch/1905108/1905108_2throughput.dat" using 1:1 title 'Speed VS ThroughPut' with linespoints     
        exit
EOF

echo ""