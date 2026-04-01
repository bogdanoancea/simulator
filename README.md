# NetEventSimulator – a Mobile Network Events Simulator

## Current Version

Current stable release: **1.3.0**

You can check the installed version with:

```bash
../Release/simulator --version
```

## Overview

Framework to run mobile network data micro-simulations.

> **Warning**
> Code revisions are expected to occur without notice.

For any questions about this software, please write to: [bogdan.oancea@gmail.com](mailto:bogdan.oancea@gmail.com)

This project implements a tool to run mobile network data micro-simulations. These simulations provide synthetic network event data that can be used to test statistical models of population counts computed from mobile network data.

By providing a kind of ground truth, the estimations obtained by using statistical models can be compared to real values that are unavailable in real life. While a simulation, no matter how sophisticated, is always different from real data, there is no reason to expect that a model would perform worse for synthetic data than for real data. On the contrary, dealing with real data would generally be expected to be even more problematic, so good performance on simulated data should be demanded anyway.

See: [Oancea et al. (2019)]
(https://cros.ec.europa.eu/system/files/2024-04/WPI_Deliverable_I2_Data_Simulator_-_A_simulator_for_network_event_data.pdf)

The code is written in C++. To build the application you need a C++ compiler compliant with **C++17**. The source code has been compiled with GNU C++ and LLVM compilers under three operating systems: **Windows**, **Linux**, and **macOS**.

---

## Building the Application on Windows

Building the simulator application on Unix-like systems (Linux, macOS, etc.) is straightforward, but on Windows this is not very easy. That is why the necessary steps are described below.

### Prerequisites

#### 1. MSYS2 environment

MSYS2 is a software distribution and building platform for Windows. The simulator was built using the MSYS2 environment provided by **RTools 4.0**, which can be downloaded from CRAN:

<https://cran.r-project.org/bin/windows/Rtools/>

After downloading the RTools installer, run it and select the installation directory.

#### 2. GNU C++ compiler on Windows with MSYS2

While there are several toolchains for building C++ applications on Windows, and Visual Studio may seem the natural choice, this project uses open-source tools only. The main reason is portability and openness. GNU C++ is available on the most commonly used platforms: Linux, macOS, and Windows.

The Windows port of GNU C/C++ is called **MinGW**. Although there are IDEs that bundle MinGW, such as CodeBlocks or Qt Creator, the following instructions focus on building from the command line.

After installing MSYS2, update all packages. In an MSYS2 shell started by double-clicking `mingw64.exe`, run:

```bash
pacman -Syu
```

Then install the toolchain and compiler:

```bash
pacman -S base-devel gcc vim cmake
```

`vim` and `cmake` are optional, but useful.

Then update the system `PATH` variable by adding the paths to the compiler and `make` utility.

Assuming the default MSYS2 location is `C:\rtools40`, you can update the path from the Windows command prompt with:

```cmd
setx path C:\rtools40\mingw64\bin;C:\rtools40\usr\bin;"%path%"
```

#### 3. GEOS C++ library

GEOS is a C++ port of JTS (Java Topology Suite). It is an open-source library that provides an object model for Euclidean planar linear geometry and is widely used in vector-based geographic information systems.

The current version of the micro-simulator was developed using **GEOS 3.7.1**, but it can also use **GEOS 3.9.1**.

The source code can be downloaded from:

<https://trac.osgeo.org/geos>

Instructions for building GEOS are available here:

<https://trac.osgeo.org/geos/wiki/BuildingOnUnixWithAutotools>

In short, open a shell by running `C:\rtools40\mingw64.exe`, go to the folder where the GEOS source code is located, and run:

```bash
./configure
make
make install
```

Compilation of the GEOS library may take several minutes.

### Build and Run the Simulation Software

Download the source code of the micro-simulator from GitHub:

```bash
git clone https://github.com/bogdanoancea/simulator.git
```

Assuming you want to download it into `D:\data-simulator`, open a Git shell in that folder and run the command above.

After the source code is downloaded, open `makefile.inc` with a text editor and change the values of the following variables:

- `PROJ_HOME`
- `GEOS_HOME`
- `MSYS_HOME`

Where:

- `PROJ_HOME` should point to the folder where you downloaded the simulator source code, for example `D:\data-simulator\simulator`
- `GEOS_HOME` should point to the folder where you installed the GEOS library, for example `C:\rtools\mingw64\lib`
- `MSYS_HOME` should point to your MSYS2 development environment, for example `C:\rtools40`

After changing these values, save the file, open an MSYS2 shell (`C:\rtools40\mingw64.exe`), go to the simulator source directory, and run:

```bash
make
make install
```

The executable is copied under the `Release` folder.

To run a simulation:

```bash
Release/simulator.exe -m map.wkt -s simulation.xml -a antennas.xml -p persons.xml -v
```

Several sample `map.wkt`, `simulation.xml`, `antennas.xml`, and `persons.xml` configuration files are provided in the `data/dataset1` to `data/dataset7` folders of the simulator source tree.

---

## Building the Application on Linux or macOS

### 1. Build GEOS

Download the GEOS C++ library from:

<https://trac.osgeo.org/geos>

Then build and install it with:

```bash
./configure
make
make install
```

If you do not have permission to install the library in the default location (`/usr/local/lib`), run:

```bash
sudo make install
```

After installation, run:

```bash
sudo ldconfig
```

### 2. Download and Build the Simulator

Clone the repository:

```bash
git clone https://github.com/bogdanoancea/simulator.git
```

Then open `makefile.inc` and change the values of:

- `PROJ_HOME`
- `GEOS_HOME`

Where:

- `PROJ_HOME` should point to the folder where you downloaded the simulator source code
- `GEOS_HOME` should point to the folder where the GEOS library was installed, usually `/usr/local/lib`

After updating these values, go to the simulator source directory and run:

```bash
make
make install
```

---

## Running the Simulator

In a terminal, run:

```bash
Release/simulator -m ./data/dataset1/map.wkt -s ./data/dataset1/simulation.xml -a ./data/dataset1/antennas.xml -p ./data/dataset1/persons.xml -pb ./data/dataset1/probabilities.xml -v
```

### Input files

The following files provide input parameters:

- `map.wkt` – the map of the simulation
- `simulation.xml` – configuration file containing the parameters of a simulation
- `antennas.xml` – antennas configuration file
- `persons.xml` – population configuration file
- `probabilities.xml` – file containing the parameters needed to compute the location probabilities

### Notes

- The location probabilities are computed only if the `-pb` parameter is given in the command line.
- If `-v` is given in the command line, the simulator will output to the console the set of persons, mobile phone operators, antennas, and mobile phones.
