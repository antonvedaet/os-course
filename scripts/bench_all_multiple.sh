#!/bin/bash

g++ -o combined stress/combined.cpp
g++ -o search stress/search_name.cpp
g++ -o path stress/short_path.cpp
bash scripts/run_bench_multiple.sh logs/search_multiple 10 ./search README.md /home/anton/itmo 5
bash scripts/run_bench_multiple.sh logs/path_multiple 10 ./path 100000
bash scripts/run_bench_multiple.sh logs/combined_multiple 10 ./combined README.md /home/anton/itmo 5 100000