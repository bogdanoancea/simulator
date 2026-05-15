#!/bin/bash
# =============================================================================
# run_benchmarks.sh
# Runs ALL NetEventSimulator benchmark scenarios and records:
#   - wall-clock time (seconds)
#   - peak RSS memory (MB)
#   - total events generated (MNO1 + MNO2)
#   - event throughput (events/second)
#
# Usage:
#   chmod +x run_benchmarks.sh
#   ./run_benchmarks.sh /path/to/Release/simulator
#
# Output:
#   results/benchmark_results.csv   — one row per scenario
#   results/benchmark.log           — timestamped run log
#
# Platform: macOS (uses /usr/bin/time -l).
#           For Linux: set TIME_FLAG="-v" and adjust parse_mem / parse_time.
# =============================================================================

SIMULATOR=${1:-"/Users/bogdanoancea/c-projects/simulator/Release/simulator"}
BENCHMARK_DIR="$(cd "$(dirname "$0")" && pwd)"
RESULTS_DIR="$(cd "$(dirname "$0")" && pwd)/results"
RESULTS_FILE="${RESULTS_DIR}/benchmark_results.csv"
LOG_FILE="${RESULTS_DIR}/benchmark.log"
TMPFILE=$(mktemp /tmp/sim_time_XXXXXX)

# ── platform detection ────────────────────────────────────────────────────────
if [[ "$(uname)" == "Darwin" ]]; then
    TIME_FLAG="-l"
    MEM_DIVISOR=1048576        # bytes → MB
    parse_mem() {
        grep "maximum resident set size" "$1" | awk '{print $1}'
    }
    parse_time() {
        grep -E "^\s+[0-9]+\.[0-9]+ real" "$1" | awk '{print $1}'
    }
else
    TIME_FLAG="-v"
    MEM_DIVISOR=1024           # kbytes → MB
    parse_mem() {
        grep "Maximum resident set size" "$1" | awk '{print $NF}'
    }
    parse_time() {
        grep "Elapsed (wall clock)" "$1" \
            | awk '{print $NF}' \
            | awk -F: '{ if (NF==2) print $1*60+$2; else print $1*3600+$2*60+$3 }'
    }
fi

# ── sanity check ──────────────────────────────────────────────────────────────
if [ ! -f "$SIMULATOR" ]; then
    echo "ERROR: simulator binary not found at: $SIMULATOR"
    echo "Usage: $0 /path/to/Release/simulator"
    exit 1
fi

mkdir -p "$RESULTS_DIR"

# Always start a fresh results file
echo "scenario,group,param_name,param_value,time_s,peak_mem_mb,n_events_mno1,n_events_mno2,n_events_total,throughput_eps" \
    > "$RESULTS_FILE"

echo "$(date): Starting full benchmark suite" | tee "$LOG_FILE"
echo "Simulator: $SIMULATOR"                  | tee -a "$LOG_FILE"
echo "Platform:  $(uname)"                    | tee -a "$LOG_FILE"
echo "Results:   $RESULTS_FILE"               | tee -a "$LOG_FILE"
echo ""                                       | tee -a "$LOG_FILE"

# ── run one scenario ──────────────────────────────────────────────────────────
run_scenario() {
    local SCENARIO_DIR="$1"
    local GROUP="$2"
    local PARAM_NAME="$3"
    local PARAM_VALUE="$4"
    local LABEL
    LABEL=$(basename "$SCENARIO_DIR")

    echo -n "  ${GROUP}/${LABEL} ... " | tee -a "$LOG_FILE"

    # clean previous outputs
    rm -rf   "${SCENARIO_DIR}/output"
    mkdir -p "${SCENARIO_DIR}/output"

    # Run simulator:
    #   /usr/bin/time writes resource stats to stderr → TMPFILE (isolated)
    #   simulator stdout is discarded
    (
        cd "$SCENARIO_DIR" || exit 1
        /usr/bin/time ${TIME_FLAG} \
            "$SIMULATOR"       \
            -m  map.wkt        \
            -s  simulation.xml \
            -a  antennas.xml   \
            -p  persons.xml    \
            > /dev/null
    ) 2> "$TMPFILE"
    local EXIT_CODE=$?

    if [ $EXIT_CODE -ne 0 ]; then
        echo "FAILED (exit $EXIT_CODE)" | tee -a "$LOG_FILE"
        echo "--- stderr dump ---"      >> "$LOG_FILE"
        cat "$TMPFILE"                  >> "$LOG_FILE"
        echo "${LABEL},${GROUP},${PARAM_NAME},${PARAM_VALUE},ERROR,ERROR,ERROR,ERROR,ERROR,ERROR" \
            >> "$RESULTS_FILE"
        return
    fi

    # parse time and memory from isolated stderr file
    local TIME_S;  TIME_S=$(parse_time "$TMPFILE");  TIME_S=${TIME_S:-0}
    local MEM_RAW; MEM_RAW=$(parse_mem  "$TMPFILE"); MEM_RAW=${MEM_RAW:-0}
    local MEM_MB;  MEM_MB=$(echo "scale=1; ${MEM_RAW} / ${MEM_DIVISOR}" | bc 2>/dev/null || echo "0")

    # count events from output CSVs
    local OUT_DIR="${SCENARIO_DIR}/output"
    local N_MNO1=0 N_MNO2=0
    for f in "${OUT_DIR}"/AntennaInfo_MNO_MNO1*.csv; do
        [[ -f "$f" ]] && N_MNO1=$(( N_MNO1 + $(wc -l < "$f") - 1 ))
    done
    for f in "${OUT_DIR}"/AntennaInfo_MNO_MNO2*.csv; do
        [[ -f "$f" ]] && N_MNO2=$(( N_MNO2 + $(wc -l < "$f") - 1 ))
    done
    local N_TOTAL=$(( N_MNO1 + N_MNO2 ))

    # throughput: events per second
    local THROUGHPUT=0
    if (( $(echo "$TIME_S > 0" | bc -l 2>/dev/null) )); then
        THROUGHPUT=$(echo "scale=0; ${N_TOTAL} / ${TIME_S}" | bc 2>/dev/null || echo "0")
    fi

    echo "done  [${TIME_S}s | ${MEM_MB} MB | ${N_TOTAL} events | ${THROUGHPUT} ev/s]" \
        | tee -a "$LOG_FILE"

    echo "${LABEL},${GROUP},${PARAM_NAME},${PARAM_VALUE},${TIME_S},${MEM_MB},${N_MNO1},${N_MNO2},${N_TOTAL},${THROUGHPUT}" \
        >> "$RESULTS_FILE"
}

# =============================================================================
# BENCHMARK GROUPS
# =============================================================================

# ── 1. Population size ────────────────────────────────────────────────────────
echo "[1] Population size" | tee -a "$LOG_FILE"
for N in 100 1000 5000 10000 50000 100000; do
    run_scenario "${BENCHMARK_DIR}/pop_size/pop_${N}" \
                 "pop_size" "n_persons" "${N}"
done

# ── 2. Simulation time ────────────────────────────────────────────────────────
echo "[2] Simulation time" | tee -a "$LOG_FILE"
for T in 900 1800 3600 7200 14400; do
    run_scenario "${BENCHMARK_DIR}/sim_time/time_${T}" \
                 "sim_time" "end_time" "${T}"
done

# ── 3. Grid size ──────────────────────────────────────────────────────────────
echo "[3] Grid size" | tee -a "$LOG_FILE"
for G in 50 100 250 500; do
    run_scenario "${BENCHMARK_DIR}/grid_size/grid_${G}" \
                 "grid_size" "tiles_per_side" "${G}"
done

# ── 4. Map size — fixed population (original experiment) ─────────────────────
echo "[4] Map size (fixed population)" | tee -a "$LOG_FILE"
for S in 10k 20k 50k 100k; do
    run_scenario "${BENCHMARK_DIR}/map_size/map_${S}" \
                 "map_size" "map_side_km" "${S%k}"
done

# ── 5. Map size — constant density (population+antennas scale with area) ─────
echo "[5] Map size (constant density)" | tee -a "$LOG_FILE"
for S in 10k 20k 50k 100k; do
    run_scenario "${BENCHMARK_DIR}/map_size_density/map_${S}_d" \
                 "map_size_density" "map_side_km" "${S%k}"
done

# ── 6. Number of antennas ─────────────────────────────────────────────────────
echo "[6] Number of antennas" | tee -a "$LOG_FILE"
for A in 22 44 66; do
    run_scenario "${BENCHMARK_DIR}/antenna_no/ant_${A}" \
                 "antenna_no" "n_antennas" "${A}"
done

# ── cleanup ───────────────────────────────────────────────────────────────────
rm -f "$TMPFILE"

echo ""                                                      | tee -a "$LOG_FILE"
echo "All benchmarks complete."                              | tee -a "$LOG_FILE"
echo "Results written to: $RESULTS_FILE"                     | tee -a "$LOG_FILE"
echo "$(date): Finished"                                     | tee -a "$LOG_FILE"
