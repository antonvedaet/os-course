#!/bin/bash

if [ $# -lt 4 ]; then
    echo "Usage: $0 <output_log_directory> <num_instances> <benchmark_program> [additional arguments]"
    exit 1
fi

OUTPUT_DIR=$1
NUM_INSTANCES=$2
BENCHMARK_PROGRAM=$3
shift 3

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

echo "Running $NUM_INSTANCES instances of $BENCHMARK_PROGRAM with arguments '$@'"
echo "Collecting performance metrics with perf stat, ltrace, strace, and top"
echo "Output will be saved in $OUTPUT_DIR directory"

run_single_benchmark() {
    local INSTANCE_ID=$1
    shift 1
    local INSTANCE_DIR="$OUTPUT_DIR/instance_$INSTANCE_ID"

    mkdir -p "$INSTANCE_DIR"

    echo "Starting benchmark instance $INSTANCE_ID..."
    echo "Logs for instance $INSTANCE_ID will be saved in $INSTANCE_DIR"

    echo "Instance $INSTANCE_ID: Running perf stat..."
    perf stat -d -d -d "$BENCHMARK_PROGRAM" "$@" 2>&1 | tee "$INSTANCE_DIR/perf.log" &

    PERF_PID=$!

    echo "Instance $INSTANCE_ID: Running ltrace..."
    ltrace -c "$BENCHMARK_PROGRAM" "$@" 2>&1 | tee "$INSTANCE_DIR/ltrace.log" &

    LTRACE_PID=$!

    echo "Instance $INSTANCE_ID: Running strace..."
    strace -c "$BENCHMARK_PROGRAM" "$@" 2>&1 | tee "$INSTANCE_DIR/strace.log" &

    STRACE_PID=$!

    wait "$PERF_PID" "$LTRACE_PID" "$STRACE_PID"

    echo "Instance $INSTANCE_ID: Benchmark and profiling completed."
}

export -f run_single_benchmark
export BENCHMARK_PROGRAM
export USER

echo "Starting top profiling..."
top -b -n3 > "$OUTPUT_DIR/top.log" &
TOP_PID=$!

BENCHMARK_PIDS=()

for i in $(seq 1 "$NUM_INSTANCES"); do
    run_single_benchmark "$i" "$@" &
    BENCHMARK_PIDS+=($!)
done

echo "Waiting for all benchmark instances to complete..."
for pid in "${BENCHMARK_PIDS[@]}"; do
    wait "$pid"
done

echo "Terminating top profiling..."
kill "$TOP_PID"

echo "All $NUM_INSTANCES benchmark instances have completed."