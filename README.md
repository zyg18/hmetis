# Hmetis - A Multilevel Hypergraph Partitioning Algorithm

## Description
modified from https://github.com/yoyojs200602/Hmetis.
used to do graph partition to minimize the cut size.

 ## Usage
 make

 ./hmetis --nleft=20 --ileft=30 --dr=0.3 --max-itr-c=30 --max-itr-r=30 --coarsen-scheme=MHEC --refine-scheme=MHEC --IP-scheme=FM --UC-scheme=FM --UC-scheme-r=FM [input_file] result.txt
