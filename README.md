# NetEventSimulator – A Mobile Network Events Simulator

## Current Version

Current stable release: **1.3.0**

You can check the installed version with:

```bash
Release/simulator -version
```

On Windows, the executable name is `Release/simulator.exe`.

## Overview

NetEventSimulator is a C++ framework for mobile network data micro-simulation. It generates synthetic network event data that can be used to test statistical models for population counts and related indicators derived from mobile network data.

A major advantage of simulation is that it provides a latent ground truth that is unavailable in real mobile network data. This makes it possible to evaluate statistical procedures under controlled conditions before applying them to real-world data.

For background, see:

- Oancea et al. (2019): <https://cros.ec.europa.eu/system/files/2024-04/WPI_Deliverable_I2_Data_Simulator_-_A_simulator_for_network_event_data.pdf>

For architecture diagrams and API-level documentation, see [ARCHITECTURE.md](ARCHITECTURE.md).

The code is written in C++ and requires a compiler with **C++17** support. The project has been built with GNU and LLVM toolchains on **Windows**, **Linux**, and **macOS**.

> **Warning**
> Code revisions may occur without notice.

For questions about the software, contact: [bogdan.oancea@gmail.com](mailto:bogdan.oancea@gmail.com)

---

## Requirements

To build the project you need:

- a C++17-capable compiler
- `make`
- the GEOS C++ library and headers
- `git` to clone the repository

NetEventSimulator currently supports **GEOS 3.10.7** as the **newest supported version**. Newer GEOS releases may build, but are not officially supported or tested.

On Windows, use a single MSYS2/UCRT64 toolchain consistently. Do **not** mix Cygwin libraries with MinGW/UCRT libraries in the same build.

---

## Building on Windows

The recommended Windows setup uses the **MSYS2 UCRT64** environment distributed with **Rtools**.

### 1. Install Rtools / MSYS2

Download Rtools from CRAN:

<https://cran.r-project.org/bin/windows/Rtools/>

Open the **UCRT64** shell after installation.

### 2. Install the compiler and tools

In the UCRT64 shell, update the package database and install the required tools:

```bash
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc 
pacman -S mingw-w64-ucrt-x86_64-make
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-ninja git
```

You may also install editors or other tools, such as `vim`, if desired.

### 3. Build and install GEOS as a static library

On Windows, **GEOS 3.10.7 is the newest supported version**. A tested configuration is a static GEOS 3.10.7 build installed under `C:/local/geos-ucrt64-static`.

Example:

```bash
rm -rf /d/c-projects/geos-3.10.7/_build

SRC=$(cygpath -m /d/c-projects/geos-3.10.7)
BLD=$(cygpath -m /d/c-projects/geos-3.10.7/_build)
INS=C:/local/geos-ucrt64-static

MSYS2_ARG_CONV_EXCL='*' /ucrt64/bin/cmake -G Ninja \
  -S "$SRC" \
  -B "$BLD" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$INS" \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DBUILD_DOCUMENTATION=OFF

MSYS2_ARG_CONV_EXCL='*' /ucrt64/bin/cmake --build "$BLD"
MSYS2_ARG_CONV_EXCL='*' /ucrt64/bin/cmake --install "$BLD"
```

Check that the static libraries were installed:

```bash
ls /c/local/geos-ucrt64-static/lib/libgeos*.a
```

### 4. Clone the repository

```bash
git clone https://github.com/bogdanoancea/neteventsimulator.git
```

### 5. Edit `makefile.inc`

Set the following variables:

- `PROJ_HOME`: simulator project root
- `GEOS_HOME`: GEOS installation prefix

Important: on Windows these paths must be written in **MSYS/UCRT style**, not in native Windows style.

Example:

```make
PROJ_HOME = /d/c-projects/neteventsimulator
GEOS_HOME = /c/local/geos-ucrt64-static
```

`GEOS_HOME` must point to the **installation prefix** that contains both `include` and `lib` subdirectories.

### 6. Build and install the simulator

From the project source directory, run:

```bash
make
make install
```

The executable is copied to the `Release` folder.

### 7. Run an example simulation

```bash
Release/simulator.exe \
  -m ./data/dataset1/map.wkt \
  -s ./data/dataset1/simulation.xml \
  -a ./data/dataset1/antennas.xml \
  -p ./data/dataset1/persons.xml \
  -pb ./data/dataset1/probabilities.xml \
  -v
```

---

## Building on Linux or macOS

### 1. Install GEOS

NetEventSimulator currently supports **GEOS 3.10.7** as the **newest supported version** on Linux and macOS. You may build GEOS 3.10.7 from source or install that version with your system package manager when available. In either case, identify the installation prefix that contains:

- `include/`
- `lib/`

Typical examples are:

- Linux: `/usr/local` or `/usr`
- macOS: `/usr/local`, `/opt/homebrew/opt/geos`, or another custom prefix

If you build GEOS from source, a standard sequence is:

```bash
./configure
make
make install
```

If needed, run:

```bash
sudo make install
sudo ldconfig
```

### 2. Clone the repository

```bash
git clone https://github.com/bogdanoancea/neteventsimulator.git
```

### 3. Edit `makefile.inc`

Set:

```make
PROJ_HOME = /path/to/neteventsimulator
GEOS_HOME = /path/to/geos/prefix
```

Examples:

```make
PROJ_HOME = /home/user/neteventsimulator
GEOS_HOME = /usr/local
```

or on Apple Silicon:

```make
PROJ_HOME = /Users/user/neteventsimulator
GEOS_HOME = /opt/homebrew/opt/geos
```

### 4. Build and install

From the project source directory, run:

```bash
make
make install
```

---

## Usage

### Running a simulation

Example:

```bash
Release/simulator \
  -m ./data/dataset1/map.wkt \
  -s ./data/dataset1/simulation.xml \
  -a ./data/dataset1/antennas.xml \
  -p ./data/dataset1/persons.xml \
  -pb ./data/dataset1/probabilities.xml \
  -v
```

On Windows, replace `Release/simulator` with `Release/simulator.exe`.

### Command-line options

| Flag | Argument | Required | Description |
|------|----------|----------|-------------|
| `-m` | path to `.wkt` file | Yes | Simulation map in Well-Known Text (WKT) format, defining the geographic area and boundary used for the simulation. |
| `-s` | path to `.xml` file | Yes | Main simulation configuration (e.g., grid/tile size, population size, simulation length, mobility model parameters). |
| `-a` | path to `.xml` file | Yes | Antenna/network configuration: number and type of antennas, operators, and technical parameters (power, path-loss exponent, azimuth/elevation patterns). |
| `-p` | path to `.xml` file | Yes | Population configuration: demographic attributes and mobile-device ownership distribution. |
| `-pb` | path to `.xml` file | No | Prior probabilities used to compute device-location probabilities. If omitted, location-probability outputs are not produced. |
| `-v` | — (no argument) | No | Verbose mode. Prints detailed information about persons, operators, antennas, and mobile phones during the run. |
| `-version` | — (no argument) | No | Prints the installed version and exits. Used standalone, without the other flags (see [Current Version](#current-version)). |

### Input files

The main input files are:

- `map.wkt` – simulation map
- `simulation.xml` – main simulation configuration
- `antennas.xml` – antenna configuration
- `persons.xml` – population configuration
- `probabilities.xml` – parameters used to compute location probabilities

Sample input files are provided under `data/dataset1` to `data/dataset7`.

### Notes

- Location probabilities are computed only if the `-pb` option is provided.
- If `-v` is provided, the simulator prints detailed information about persons, operators, antennas, and mobile phones.

---

## Troubleshooting

- If compilation succeeds but linking fails with unresolved C++ runtime symbols, verify that you are not mixing toolchains.
- On Windows, build both GEOS and the simulator with the same UCRT64 toolchain.
- `GEOS_HOME` should be the installation prefix, not the `include` directory and not the `lib` directory itself.
- If you use a newer GEOS release, note that **GEOS 3.10.7 is the newest officially supported version** of the simulator.
- On macOS, if the simulator fails at runtime because the GEOS shared library cannot be found, run `export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH` before starting the simulator, adjusting `/usr/local/lib` if GEOS is installed in a different location.
- If the compiler command shows a non-existent include directory, enable `-Wmissing-include-dirs` to catch it early.
