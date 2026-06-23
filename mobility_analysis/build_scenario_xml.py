#!/usr/bin/env python3
"""
build_scenario_xml.py
----------------------
Builds a scenario simulation.xml by starting from a fixed baseline file
and (optionally) splicing in the <movement_pattern> block from a
*different* dataset's simulation.xml, then applying tag-value overrides.

This lets every mobility-model scenario share exactly the same map,
antennas, population, and simulation length (whatever the baseline
defines), varying ONLY the movement_pattern -- which is the controlled,
single-factor comparison needed for the mobility-model sensitivity
analysis.

Usage:
    python3 build_scenario_xml.py \\
        --base BASELINE_SIMULATION_XML \\
        --out OUTPUT_XML \\
        [--source-pattern SOURCE_SIMULATION_XML] \\
        [--set path/to/tag=value ...]

If --source-pattern is omitted, the baseline's own <movement_pattern>
block is kept unchanged (only --set overrides are applied) -- this is
the right mode for sweeping a parameter of the baseline's own model
(e.g. random_walk_closed_map_drift, which IS dataset1's native model).

If --source-pattern is given, the baseline's <movement_pattern> element
is replaced by the <movement_pattern> element found in that file before
any --set overrides are applied -- this is the right mode for splicing
in a different mobility model (e.g. levy_flight, manhattan, home_work)
while keeping the rest of the baseline (map/antennas/population/etc.)
fixed.

Examples:
    # sweep the baseline's own drift model
    python3 build_scenario_xml.py --base dataset1/simulation.xml \\
        --out out/simulation.xml \\
        --set movement_pattern/trend_angle_1_distribution/sd=60 \\
        --set movement_pattern/trend_angle_2_distribution/sd=60

    # splice in the Levy-flight model from dataset3, then set its scale
    python3 build_scenario_xml.py --base dataset1/simulation.xml \\
        --out out/simulation.xml \\
        --source-pattern dataset3/simulation.xml \\
        --set movement_pattern/speed_distribution/c=30
"""
import argparse
import sys
import xml.etree.ElementTree as ET


def set_text(root, path, value):
    el = root.find(path)
    if el is None:
        raise SystemExit(f"ERROR: tag path not found: '{path}'")
    el.text = str(value)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                  formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--base", required=True, help="baseline simulation.xml")
    ap.add_argument("--out", required=True, help="output simulation.xml path")
    ap.add_argument("--source-pattern", default=None,
                     help="simulation.xml whose <movement_pattern> block replaces the baseline's")
    ap.add_argument("--set", action="append", default=[],
                     metavar="PATH=VALUE",
                     help="override a tag's text, e.g. movement_pattern/.../sd=60 "
                          "(repeatable)")
    args = ap.parse_args()

    tree = ET.parse(args.base)
    root = tree.getroot()

    if args.source_pattern:
        src_root = ET.parse(args.source_pattern).getroot()
        new_mp = src_root.find("movement_pattern")
        if new_mp is None:
            raise SystemExit(f"ERROR: no <movement_pattern> found in {args.source_pattern}")

        old_mp = root.find("movement_pattern")
        if old_mp is None:
            raise SystemExit(f"ERROR: no <movement_pattern> found in baseline {args.base}")

        children = list(root)
        idx = children.index(old_mp)
        root.remove(old_mp)
        root.insert(idx, new_mp)

    for ov in args.set:
        if "=" not in ov:
            raise SystemExit(f"ERROR: malformed override (expected path=value): '{ov}'")
        path, value = ov.split("=", 1)
        set_text(root, path, value)

    tree.write(args.out, encoding="unicode")
    print(f"wrote {args.out}")


if __name__ == "__main__":
    main()
