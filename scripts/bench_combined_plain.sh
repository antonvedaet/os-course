#!/bin/bash

g++ -o combined stress/combined.cpp
bash scripts/run_bench.sh logs/combined_plain ./combined README.md /home/anton/itmo 5 100000