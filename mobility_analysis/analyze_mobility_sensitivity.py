#!/usr/bin/env python3
"""
analyze_mobility_sensitivity.py
--------------------------------
Consolidates the outputs of run_mobility_sensitivity.sh into a single
results CSV with spatial-dispersion and event-count metrics, for use in
the manuscript's mobility-model parameter-sensitivity discussion.

For each scenario directory <root>/<group>/<label>/ it reads:
  - simulation.xml   (to recover the actual parameter value(s) used)
  - output/persons.csv                  (ground-truth trajectories)
  - output/grid.csv                     (ONE summary row: Origin X/Y,
                                          X/Y Tile Dim, No Tiles X/Y --
                                          used for both tile geometry and
                                          the total tile count)
  - output/AntennaInfo_MNO_MNO1*.csv    (event counts, operator 1)
  - output/AntennaInfo_MNO_MNO2*.csv    (event counts, operator 2)

and computes:
  - tiles_total, tiles_visited, pct_tiles_visited
  - mean_net_displacement   (straight-line distance from first to last
                              recorded position, averaged over persons)
  - mean_path_length        (sum of consecutive step distances, averaged
                              over persons)
  - n_events_mno1, n_events_mno2, n_events_total

Usage:
    python3 analyze_mobility_sensitivity.py <mobility_sensitivity_root> \
        [--id-col NAME] [--time-col NAME] [--x-col NAME] [--y-col NAME]

If column auto-detection fails for persons.csv, the script prints the
actual header it found and exits, so the column names can be supplied
explicitly with the flags above.
"""
import argparse
import csv
import glob
import math
import os
import re
import sys
import xml.etree.ElementTree as ET

ID_CANDIDATES = ["person_id", "personid", "id_person", "id", "person", "person id"]
TIME_CANDIDATES = ["time", "time_step", "timestep", "t", "time_increment_no"]
X_CANDIDATES = ["x", "pos_x", "x_coord", "xcoord", "coord_x"]
Y_CANDIDATES = ["y", "pos_y", "y_coord", "ycoord", "coord_y"]
TILE_CANDIDATES = ["tile id", "tile_id", "tileid"]


def detect_column(header, candidates, explicit=None):
    if explicit:
        if explicit in header:
            return explicit
        raise SystemExit(f"ERROR: explicit column '{explicit}' not found in header: {header}")
    lower = {h.lower(): h for h in header}
    for c in candidates:
        if c in lower:
            return lower[c]
    return None


def parse_grid_csv(path):
    """grid.csv contains exactly ONE data row summarising the whole grid:
    Origin X, Origin Y, X Tile Dim, Y Tile Dim, No Tiles X, No Tiles Y.
    This is the authoritative source for tile geometry and tile count --
    prefer it over guessing from map.wkt / simulation.xml."""
    with open(path, "r", encoding="utf-8", errors="ignore") as fh:
        reader = csv.DictReader(fh)
        row = next(reader, None)
    if row is None:
        raise SystemExit(f"ERROR: {path} has no data row")
    origin_x = float(row["Origin X"])
    origin_y = float(row["Origin Y"])
    tile_x = float(row["X Tile Dim"])
    tile_y = float(row["Y Tile Dim"])
    n_tiles_x = int(float(row["No Tiles X"]))
    n_tiles_y = int(float(row["No Tiles Y"]))
    return origin_x, origin_y, tile_x, tile_y, n_tiles_x * n_tiles_y


NATIVE_PARAM_PATH = {
    "random_walk_closed_map_drift": "movement_pattern/trend_angle_1_distribution/sd",
    "levy_flight": "movement_pattern/speed_distribution/c",
    "manhattan": "movement_pattern/manhattan_grid/x_step",
    "home_work": "movement_pattern/direction_angle_distribution/scale",
    "home_work_manhattan": "movement_pattern/manhattan_grid/x_step",
    # random_walk_closed_map has no native parameter -> no entry
}

UNIVERSAL_PARAM_PATH = {
    "stay_interval": "interval_between_stays_distribution/mean",
    "time_stay": "time_stay_distribution/mean",
    "replicate": "random_seed",
}

# back-compat with the earlier flat (group-only) layout, where "group" was
# e.g. "drift_sd" or "stay_interval" with no separate sweep_kind directory
LEGACY_GROUP_PARAM_PATH = {
    "drift_sd": "movement_pattern/trend_angle_1_distribution/sd",
    "stay_interval": "interval_between_stays_distribution/mean",
}


def get_param_value(sim_xml_path, movement_pattern_type, sweep_kind):
    root = ET.parse(sim_xml_path).getroot()
    if sweep_kind in UNIVERSAL_PARAM_PATH:
        path = UNIVERSAL_PARAM_PATH[sweep_kind]
    elif sweep_kind == "native":
        path = NATIVE_PARAM_PATH.get(movement_pattern_type)
    else:
        path = LEGACY_GROUP_PARAM_PATH.get(sweep_kind)  # legacy flat layout
    if path is None:
        return "NA"  # parameter-free reference runs (random_walk_closed_map/native)
    el = root.find(path)
    return el.text if el is not None else "NA"


def get_movement_pattern_type(sim_xml_path):
    root = ET.parse(sim_xml_path).getroot()
    el = root.find("movement_pattern")
    return el.get("type") if el is not None else "NA"


def count_csv_rows(path_glob):
    total = 0
    for f in glob.glob(path_glob):
        with open(f, "r", encoding="utf-8", errors="ignore") as fh:
            n = sum(1 for _ in fh)
        total += max(n - 1, 0)  # subtract header
    return total


def analyze_persons_csv(path, xmin, ymin, tile_x, tile_y, id_col, time_col, x_col, y_col, tile_col=None):
    with open(path, "r", encoding="utf-8", errors="ignore") as fh:
        reader = csv.reader(fh)
        header = next(reader)

        id_c = detect_column(header, ID_CANDIDATES, id_col)
        time_c = detect_column(header, TIME_CANDIDATES, time_col)
        x_c = detect_column(header, X_CANDIDATES, x_col)
        y_c = detect_column(header, Y_CANDIDATES, y_col)
        # Prefer the simulator's own tile assignment when available: it's
        # more reliable than re-deriving tiles from a guessed map origin.
        tile_c = detect_column(header, TILE_CANDIDATES, tile_col)

        missing = [n for n, v in
                   [("id", id_c), ("time", time_c), ("x", x_c), ("y", y_c)] if v is None]
        if missing:
            print(f"ERROR: could not auto-detect column(s) {missing} in {path}", file=sys.stderr)
            print(f"Actual header: {header}", file=sys.stderr)
            print("Re-run with --id-col / --time-col / --x-col / --y-col to specify "
                  "the correct column names.", file=sys.stderr)
            sys.exit(1)

        idx = {h: i for i, h in enumerate(header)}
        tiles_visited = set()
        # per-person ordered list of (time, x, y)
        persons = {}

        for row in reader:
            if not row:
                continue
            pid = row[idx[id_c]]
            t = float(row[idx[time_c]])
            x = float(row[idx[x_c]])
            y = float(row[idx[y_c]])
            if tile_c is not None:
                tile = row[idx[tile_c]]
            else:
                tile = (int((x - xmin) // tile_x), int((y - ymin) // tile_y))
            tiles_visited.add(tile)
            persons.setdefault(pid, []).append((t, x, y))

    net_displacements = []
    path_lengths = []
    for pid, pts in persons.items():
        pts.sort(key=lambda p: p[0])
        if len(pts) < 2:
            continue
        _, x0, y0 = pts[0]
        _, x1, y1 = pts[-1]
        net_displacements.append(math.hypot(x1 - x0, y1 - y0))

        total_len = 0.0
        for (_, xa, ya), (_, xb, yb) in zip(pts[:-1], pts[1:]):
            total_len += math.hypot(xb - xa, yb - ya)
        path_lengths.append(total_len)

    mean_disp = sum(net_displacements) / len(net_displacements) if net_displacements else 0.0
    mean_path = sum(path_lengths) / len(path_lengths) if path_lengths else 0.0

    return len(tiles_visited), mean_disp, mean_path


def find_scenario_dirs(root):
    """Finds every scenario directory (one containing both simulation.xml
    and an output/ subfolder) anywhere under root, and returns it together
    with its path components relative to root. Supports both:
      - new layout:    <pattern>/<sweep_kind>/<label>/   (3 components)
      - legacy layout: <group>/<label>/                  (2 components)
    """
    found = []
    for dirpath, dirnames, filenames in os.walk(root):
        if "simulation.xml" in filenames and "output" in dirnames:
            rel = os.path.relpath(dirpath, root)
            parts = rel.split(os.sep)
            found.append((dirpath, parts))
            dirnames[:] = []  # don't descend further into a scenario dir
    return found


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("root", help="mobility_sensitivity root directory")
    ap.add_argument("--id-col", default=None)
    ap.add_argument("--time-col", default=None)
    ap.add_argument("--x-col", default=None)
    ap.add_argument("--y-col", default=None)
    ap.add_argument("--tile-col", default=None,
                     help="column holding the simulator's own tile assignment "
                          "(preferred over re-deriving tiles from map bounding box)")
    ap.add_argument("--out", default=None, help="output CSV path (default: <root>/mobility_sensitivity_results.csv)")
    args = ap.parse_args()

    root = args.root
    out_path = args.out or os.path.join(root, "mobility_sensitivity_results.csv")

    rows = []
    for scen_dir, parts in sorted(find_scenario_dirs(root)):
        if len(parts) == 3:
            pattern, sweep_kind, label = parts
        elif len(parts) == 2:
            pattern, label = parts
            sweep_kind = pattern  # legacy: group name doubles as the sweep_kind key
        else:
            print(f"WARNING: unexpected scenario depth, skipping: {scen_dir}", file=sys.stderr)
            continue

        out_dir = os.path.join(scen_dir, "output")
        sim_xml = os.path.join(scen_dir, "simulation.xml")
        persons_csv = os.path.join(out_dir, "persons.csv")
        grid_csv = os.path.join(out_dir, "grid.csv")

        if not os.path.isfile(persons_csv):
            print(f"WARNING: skipping incomplete scenario {scen_dir}", file=sys.stderr)
            continue
        if not os.path.isfile(grid_csv):
            print(f"WARNING: skipping scenario with no grid.csv: {scen_dir}", file=sys.stderr)
            continue

        origin_x, origin_y, tile_x, tile_y, tiles_total = parse_grid_csv(grid_csv)
        mp_type = get_movement_pattern_type(sim_xml)
        param_value = get_param_value(sim_xml, mp_type, sweep_kind)

        tiles_visited, mean_disp, mean_path = analyze_persons_csv(
            persons_csv, origin_x, origin_y, tile_x, tile_y,
            args.id_col, args.time_col, args.x_col, args.y_col, args.tile_col
        )

        n_mno1 = count_csv_rows(os.path.join(out_dir, "AntennaInfo_MNO_MNO1*.csv"))
        n_mno2 = count_csv_rows(os.path.join(out_dir, "AntennaInfo_MNO_MNO2*.csv"))

        pct_visited = (100.0 * tiles_visited / tiles_total) if tiles_total else 0.0

        rows.append({
            "group": pattern,
            "movement_pattern_type": mp_type,
            "sweep_kind": sweep_kind,
            "scenario": label,
            "param_value": param_value,
            "tiles_total": tiles_total,
            "tiles_visited": tiles_visited,
            "pct_tiles_visited": round(pct_visited, 1),
            "mean_net_displacement": round(mean_disp, 1),
            "mean_path_length": round(mean_path, 1),
            "n_events_mno1": n_mno1,
            "n_events_mno2": n_mno2,
            "n_events_total": n_mno1 + n_mno2,
        })

        print(f"  {pattern}/{sweep_kind}/{label}: tiles={tiles_visited}/{tiles_total} "
              f"disp={mean_disp:.1f} path={mean_path:.1f} "
              f"events={n_mno1 + n_mno2}")

    if not rows:
        raise SystemExit("ERROR: no complete scenarios found under " + root)

    fieldnames = list(rows[0].keys())
    with open(out_path, "w", newline="") as fh:
        writer = csv.DictWriter(fh, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)

    print(f"\nWrote consolidated results to: {out_path}")


if __name__ == "__main__":
    main()
