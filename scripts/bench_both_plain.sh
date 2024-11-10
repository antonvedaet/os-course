#!/bin/bash

g++ -o search stress/search_name.cpp
g++ -o path stress/short_path.cpp
bash scripts/run_bench.sh logs/search_plain ./search README.md /home/anton/itmo 5
bash scripts/run_bench.sh logs/path_plain ./path 100000
