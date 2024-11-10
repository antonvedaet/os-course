#!/bin/bash

g++ -o combined -O3 stress/combined.cpp
bash scripts/run_bench.sh logs/combined_aggressive ./combined README.md /home/anton/itmo 5 100000