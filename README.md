```text
# ns-3 Experiments Collection
A small collection of ns-3 example simulations, a tiny helper application, parsing/plotting scripts, and NetAnim/FlowMonitor outputs used to study:
UDP flows over a small mesh of point-to-point links (flow characterization with FlowMonitor).
TCP behavior (congestion window tracing).
CSMA + Wi‑Fi + P2P combined examples (from the other example scripts).
It includes automation to extract flow/bytes/time data from FlowMonitor XML and create publication-ready plots (gnuplot), and a GitHub Actions workflow to run the parser+plot on push.

This repository collects several ns-3 example simulations, helper code and plotting scripts used to produce traces and figures (NetAnim XML, FlowMonitor XML, pcap/ascii traces, and gnuplot PNGs).

Quick contents
- Examples (.cc/.h): myfirst.cc, mysecond.cc, mythird.cc, fourth.cc, fifth.cc, tutorial-app.{cc,h}
- Parser & plotting helpers: parse_flows.sh, prasadma_bytes.plt, task3plot.plt, atharva_*.plt
- Orchestration scripts: scripts/run_all.sh, scripts/parse_and_plot.sh
- NetAnim / FlowMonitor outputs created by simulations: prasadma_4.xml, prasadma_4_flow.xml, prasadma.dat, pcap/tr files (these are generated artifacts and are ignored by .gitignore)

Prerequisites
- ns-3 (tested with recent ns-3 releases)
- gnuplot (for .plt scripts)
- bash, awk, sed, python3
- (Optional) GitHub CLI `gh` to create repo from terminal

How to add to ns-3 (quick)
Option A — copy to ns-3 scratch (quick):
  cp *.cc *.h ~/ns-allinone-3.xx/ns-3.xx/scratch/
  cp parse_flows.sh prasadma_bytes.plt task3plot.plt *.plt ~/ns-allinone-3.xx/ns-3.xx/

Build & run (from ns-3 top-level)
  ./waf                       # build (if needed)
  ./waf --run scratch/fourth  # run the 4-node UDP flows -> prasadma_4_flow.xml output
  ./waf --run scratch/fifth   # run TCP cwnd experiment -> prasadma.dat output

Parse & plot
  ./parse_flows.sh            # requires prasadma_4_flow.xml in current dir
  gnuplot prasadma_bytes.plt  # produces prasadma_flows.png
  gnuplot task3plot.plt       # produces cwnd_plot.png (needs prasadma.dat)

Simple local automation
  bash scripts/run_all.sh        # runs the two main scenarios via waf (assumes you're in ns-3 top)
  bash scripts/parse_and_plot.sh # parse and plot (expects prasadma_4_flow.xml and prasadma.dat)


Notes
- parse_flows.sh now uses FlowProbe points when present and creates a richer prasadma_flowN.dat time-series for plotting.
- Large generated artifacts (pcap, .tr, .xml, .png, prasadma.dat) are included in .gitignore by default to keep repo small. If you want to check any of them in, remove them from .gitignore before committing.
```
