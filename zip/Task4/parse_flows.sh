#!/bin/bash
# Parse FlowMonitor XML to extract (time, rxBytes) for Flow 1 and Flow 2

XML_FILE="prasadma_4_flow.xml"

# Flow 1 (A->B) data extraction
grep 'flowId="1"' "$XML_FILE" | sed -n \
  's/.*timeFirstTxPacket="\([0-9.e+-]*\)ns".*timeLastRxPacket="\([0-9.e+-]*\)ns".*rxBytes="\([0-9]*\)".*/\1 \3 \2/p' \
  | awk '{start=$1/1000000000; end=$3/1000000000; bytes=$2; printf "%.3f %d\n%.3f %d\n", start, 0, end, bytes;}' \
  > prasadma_flow1.dat

# Flow 2 (A->D) data extraction
grep 'flowId="2"' "$XML_FILE" | sed -n \
  's/.*timeFirstTxPacket="\([0-9.e+-]*\)ns".*timeLastRxPacket="\([0-9.e+-]*\)ns".*rxBytes="\([0-9]*\)".*/\1 \3 \2/p' \
  | awk '{start=$1/1000000000; end=$3/1000000000; bytes=$2; printf "%.3f %d\n%.3f %d\n", start, 0, end, bytes;}' \
  > prasadma_flow2.dat

echo "Extracted data points into prasadma_flow1.dat and prasadma_flow2.dat."
