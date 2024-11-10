#!/bin/bash


if [ $# -lt 3 ]; then
    echo "Usage: $0 <output_log_directory> <benchmark_program> [additional_arguments...]"
    exit 1
fi

OUTPUT_DIR=$1
BENCHMARK_PROGRAM=$2
shift 2

mkdir -p "$OUTPUT_DIR"

if [ ! -f "$BENCHMARK_PROGRAM" ]; then
    echo "Error: Benchmark program $BENCHMARK_PROGRAM not found."
    exit 1
fi

if [ ! -x "$BENCHMARK_PROGRAM" ]; then
    echo "Error: Benchmark program $BENCHMARK_PROGRAM is not executable."
    exit 1
fi

USER=anton 

echo "Running $BENCHMARK_PROGRAM with arguments '$@'"
echo "Collecting performance metrics with perf stat, ltrace, strace, top, and FlameGraph"
echo "Output will be saved in $OUTPUT_DIR directory"

echo "Running perf stat..."
perf stat -d -d -d "$BENCHMARK_PROGRAM" "$@" 2>&1 | tee "$OUTPUT_DIR/perf.log"

echo "Running ltrace..."
ltrace -c "$BENCHMARK_PROGRAM" "$@" 2>&1 | tee "$OUTPUT_DIR/ltrace.log"

echo "Running strace..."
strace -c "$BENCHMARK_PROGRAM" "$@" 2>&1 | tee "$OUTPUT_DIR/strace.log"

echo "Running top..."
"$BENCHMARK_PROGRAM" "$@" & 
PID=$!
top -b -n3 > "$OUTPUT_DIR/top.log"
wait $PID    

FLAMEGRAPH_SCRIPT="/home/$USER/FlameGraph/flamegraph.pl"
FLAMEGRAPH_COLLAPSE="/home/$USER/FlameGraph/stackcollapse-perf.pl"

echo "Generating FlameGraph..."
perf record -F 144 -g "$BENCHMARK_PROGRAM" "$@"
perf script | "$FLAMEGRAPH_COLLAPSE" > "$OUTPUT_DIR/perf-folded.out"

if command -v "$FLAMEGRAPH_SCRIPT" >/dev/null 2>&1; then
    "$FLAMEGRAPH_SCRIPT" "$OUTPUT_DIR/perf-folded.out" > "$OUTPUT_DIR/flamegraph.svg"
    echo "FlameGraph saved to $OUTPUT_DIR/flamegraph.svg"
else
    echo "FlameGraph script not found."
fi

rm -f perf.data "$OUTPUT_DIR/perf-folded.out"
