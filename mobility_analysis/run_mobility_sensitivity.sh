#!/bin/bash
# =============================================================================
# run_mobility_sensitivity.sh  (v3 — native + universal sweeps, all 6 patterns)
#
# For each of the six mobility patterns implemented in NetEventSimulator,
# runs:
#   (1) a sweep of that pattern's own native parameter (where it has one)
#   (2) a sweep of interval_between_stays_distribution/mean (universal --
#       applies regardless of movement_pattern type)
#   (3) a sweep of time_stay_distribution/mean (universal)
#   (4) 5 replicate runs at the pattern's own DEFAULT configuration, varying
#       only random_seed -- isolates pure simulation stochastic noise from
#       any real parameter effect seen in (1)-(3)
#
# All scenarios share a single fixed baseline (map, antennas, population,
# probabilities, simulation length) from data/dataset1. Only the
# <movement_pattern> block and the ONE parameter under test differ between
# scenarios -- a controlled, single-factor comparison throughout.
#
# Native parameter per pattern:
#   random_walk_closed_map        - none (parameter-free)
#   random_walk_closed_map_drift  - trend-angle sd (deg.):      5, 20, 60, 120
#   levy_flight                   - speed-distribution scale c: 2, 10, 30, 80
#   manhattan                     - grid step (x_step=y_step):  25, 50, 100, 250
#   home_work                     - direction_angle_distribution/scale: 2,15,40,80
#   home_work_manhattan           - grid step (x_step=y_step):  25, 50, 100, 250
#
# Universal sweeps (run for ALL six patterns, native parameter held at its
# own default while these are varied):
#   stay_interval - interval_between_stays_distribution/mean: 10, 25, 50, 100
#   time_stay     - time_stay_distribution/mean:               0, 10, 30, 60
#   replicate     - random_seed (all else at default):         1, 2, 3, 4, 5
#
# Usage:
#   chmod +x run_mobility_sensitivity.sh
#   ./run_mobility_sensitivity.sh /path/to/Release/simulator /path/to/repo/data
#
# ($2 must contain dataset1 .. dataset7, i.e. the repo's "data" folder.)
#
# Output:
#   mobility_sensitivity/<pattern>/<sweep_kind>/<label>/output/  — raw outputs
#   mobility_sensitivity/mobility_sensitivity_raw.log            — run log
#
# Total runs: 6 patterns x (4 native [0 for the parameter-free pattern] +
#             4 stay_interval + 4 time_stay + 5 replicate) = 98 runs
#
# After this script finishes, run:
#   python3 analyze_mobility_sensitivity.py mobility_sensitivity
# =============================================================================

SIMULATOR=${1:-"/Users/bogdanoancea/c-projects/simulator/Release/simulator"}
case "$SIMULATOR" in
    /*) : ;;
    *) SIMULATOR="$(cd "$(dirname "$SIMULATOR")" && pwd)/$(basename "$SIMULATOR")" ;;
esac
DATA_DIR=${2:-"./data"}
BASELINE_DIR="${DATA_DIR}/dataset1"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT_ROOT="${SCRIPT_DIR}/mobility_sensitivity"
LOG_FILE="${OUT_ROOT}/mobility_sensitivity_raw.log"

# ── sanity checks ────────────────────────────────────────────────────────────
if [ ! -f "$SIMULATOR" ]; then
    echo "ERROR: simulator binary not found at: $SIMULATOR"
    echo "Usage: $0 /path/to/Release/simulator /path/to/repo/data"
    exit 1
fi

for f in map.wkt persons.xml antennas.xml probabilities.xml simulation.xml; do
    if [ ! -f "${BASELINE_DIR}/${f}" ]; then
        echo "ERROR: baseline file not found: ${BASELINE_DIR}/${f}"
        exit 1
    fi
done

for d in dataset2 dataset3 dataset5 dataset6 dataset7; do
    if [ ! -f "${DATA_DIR}/${d}/simulation.xml" ]; then
        echo "ERROR: required source pattern file not found: ${DATA_DIR}/${d}/simulation.xml"
        exit 1
    fi
done

mkdir -p "$OUT_ROOT"
echo "$(date): Starting full mobility-pattern sensitivity sweep" | tee "$LOG_FILE"
echo "Simulator: $SIMULATOR"                                      | tee -a "$LOG_FILE"
echo "Data dir:  $DATA_DIR"                                       | tee -a "$LOG_FILE"
echo "Baseline:  $BASELINE_DIR (map/antennas/population/probabilities fixed)" | tee -a "$LOG_FILE"
echo "Output:    $OUT_ROOT"                                       | tee -a "$LOG_FILE"
echo ""                                                            | tee -a "$LOG_FILE"

# ── build and run one scenario ────────────────────────────────────────────────
# args: pattern  sweep_kind  label  source_pattern_or_BASE  [override1 ...]
run_scenario() {
    local PATTERN="$1"
    local SWEEP_KIND="$2"
    local LABEL="$3"
    local SRC_PATTERN="$4"
    shift 4
    local OVERRIDES=("$@")

    local SCEN_DIR="${OUT_ROOT}/${PATTERN}/${SWEEP_KIND}/${LABEL}"
    mkdir -p "$SCEN_DIR"

    cp "${BASELINE_DIR}/map.wkt"           "${SCEN_DIR}/map.wkt"
    cp "${BASELINE_DIR}/persons.xml"       "${SCEN_DIR}/persons.xml"
    cp "${BASELINE_DIR}/antennas.xml"      "${SCEN_DIR}/antennas.xml"
    cp "${BASELINE_DIR}/probabilities.xml" "${SCEN_DIR}/probabilities.xml"

    local BUILD_ARGS=(--base "${BASELINE_DIR}/simulation.xml"
                       --out  "${SCEN_DIR}/simulation.xml"
                       --set  "output_dir=output")
    if [ "$SRC_PATTERN" != "BASE" ]; then
        BUILD_ARGS+=(--source-pattern "$SRC_PATTERN")
    fi
    for ov in "${OVERRIDES[@]}"; do
        BUILD_ARGS+=(--set "$ov")
    done

    python3 "${SCRIPT_DIR}/build_scenario_xml.py" "${BUILD_ARGS[@]}" >> "$LOG_FILE"

    echo -n "  ${PATTERN}/${SWEEP_KIND}/${LABEL} ... " | tee -a "$LOG_FILE"

    rm -rf   "${SCEN_DIR}/output"
    mkdir -p "${SCEN_DIR}/output"

    (
        cd "$SCEN_DIR" || exit 1
        "$SIMULATOR" \
            -m  map.wkt        \
            -s  simulation.xml \
            -a  antennas.xml   \
            -p  persons.xml    \
            > run_stdout.log 2>&1
    )
    local EXIT_CODE=$?

    if [ $EXIT_CODE -ne 0 ]; then
        echo "FAILED (exit $EXIT_CODE)" | tee -a "$LOG_FILE"
        echo "--- stdout/stderr dump ---" >> "$LOG_FILE"
        cat "${SCEN_DIR}/run_stdout.log"   >> "$LOG_FILE"
    else
        echo "done" | tee -a "$LOG_FILE"
    fi
}

# args: pattern  source_pattern_or_BASE
run_universal_sweeps() {
    local PATTERN="$1"
    local SRC_PATTERN="$2"

    for M in 10 25 50 100; do
        run_scenario "$PATTERN" "stay_interval" "mean_${M}" "$SRC_PATTERN" \
            "interval_between_stays_distribution/mean=${M}"
    done
    for T in 0 10 30 60; do
        run_scenario "$PATTERN" "time_stay" "mean_${T}" "$SRC_PATTERN" \
            "time_stay_distribution/mean=${T}"
    done
}

# args: pattern  source_pattern_or_BASE
# Replicate runs at the pattern's own DEFAULT configuration (no native or
# universal-parameter override) -- only random_seed differs. This isolates
# pure simulation stochastic noise from any real parameter effect, since
# nothing else changes across these runs.
run_replicates() {
    local PATTERN="$1"
    local SRC_PATTERN="$2"

    for SEED in 1 2 3 4 5; do
        run_scenario "$PATTERN" "replicate" "seed_${SEED}" "$SRC_PATTERN" \
            "random_seed=${SEED}"
    done
}

# =============================================================================
# [1] random_walk_closed_map — no native parameter; universal sweeps only
# =============================================================================
echo "[1] random_walk_closed_map" | tee -a "$LOG_FILE"
run_universal_sweeps "random_walk_closed_map" "${DATA_DIR}/dataset2/simulation.xml"
run_replicates "random_walk_closed_map" "${DATA_DIR}/dataset2/simulation.xml"

# =============================================================================
# [2] random_walk_closed_map_drift — native: trend-angle sd
# =============================================================================
echo "[2] random_walk_closed_map_drift" | tee -a "$LOG_FILE"
for SD in 5 20 60 120; do
    run_scenario "random_walk_closed_map_drift" "native" "sd_${SD}" "BASE" \
        "movement_pattern/trend_angle_1_distribution/sd=${SD}" \
        "movement_pattern/trend_angle_2_distribution/sd=${SD}"
done
run_universal_sweeps "random_walk_closed_map_drift" "BASE"
run_replicates "random_walk_closed_map_drift" "BASE"

# =============================================================================
# [3] levy_flight — native: speed-distribution scale c
# =============================================================================
echo "[3] levy_flight" | tee -a "$LOG_FILE"
for C in 2 10 30 80; do
    run_scenario "levy_flight" "native" "c_${C}" "${DATA_DIR}/dataset3/simulation.xml" \
        "movement_pattern/speed_distribution/c=${C}"
done
run_universal_sweeps "levy_flight" "${DATA_DIR}/dataset3/simulation.xml"
run_replicates "levy_flight" "${DATA_DIR}/dataset3/simulation.xml"

# =============================================================================
# [4] manhattan — native: grid step
# =============================================================================
echo "[4] manhattan" | tee -a "$LOG_FILE"
for STEP in 25 50 100 250; do
    run_scenario "manhattan" "native" "step_${STEP}" "${DATA_DIR}/dataset6/simulation.xml" \
        "movement_pattern/manhattan_grid/x_step=${STEP}" \
        "movement_pattern/manhattan_grid/y_step=${STEP}"
done
run_universal_sweeps "manhattan" "${DATA_DIR}/dataset6/simulation.xml"
run_replicates "manhattan" "${DATA_DIR}/dataset6/simulation.xml"

# =============================================================================
# [5] home_work — native: direction_angle_distribution/scale
# =============================================================================
echo "[5] home_work" | tee -a "$LOG_FILE"
for SCALE in 2 15 40 80; do
    run_scenario "home_work" "native" "scale_${SCALE}" "${DATA_DIR}/dataset5/simulation.xml" \
        "movement_pattern/direction_angle_distribution/scale=${SCALE}"
done
run_universal_sweeps "home_work" "${DATA_DIR}/dataset5/simulation.xml"
run_replicates "home_work" "${DATA_DIR}/dataset5/simulation.xml"

# =============================================================================
# [6] home_work_manhattan — native: grid step
# =============================================================================
echo "[6] home_work_manhattan" | tee -a "$LOG_FILE"
for STEP in 25 50 100 250; do
    run_scenario "home_work_manhattan" "native" "step_${STEP}" "${DATA_DIR}/dataset7/simulation.xml" \
        "movement_pattern/manhattan_grid/x_step=${STEP}" \
        "movement_pattern/manhattan_grid/y_step=${STEP}"
done
run_universal_sweeps "home_work_manhattan" "${DATA_DIR}/dataset7/simulation.xml"
run_replicates "home_work_manhattan" "${DATA_DIR}/dataset7/simulation.xml"

echo ""                                                                | tee -a "$LOG_FILE"
echo "All mobility-pattern scenarios complete (98 runs)."              | tee -a "$LOG_FILE"
echo "Next step: python3 analyze_mobility_sensitivity.py ${OUT_ROOT}"  | tee -a "$LOG_FILE"
echo "$(date): Finished"                                               | tee -a "$LOG_FILE"
