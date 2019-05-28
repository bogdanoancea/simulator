# Simulator
<b> Framework to run mobile network data micro-simulations </b>
<p>
<p>!!! Warning !!!</p>
This project is in early stages of development. 
Major code revisions are expected to occur without notice and on a daily basis.
</p>

<p>
The project implements a "development of a framework to run mobile network data micro-simulations. 
These simulations will provide us with synthetic data, useful to test the models. 
Being a micro-simulation, the estimations can be compared to the real data, unavailable in real life. 
While a simulation is (no matter how sophisticated) always different from real data, 
there is really no reason to expect that a model would perform worse for synthetic data than for real data. 
On the contrary, dealing with real data would be expected to be even more problematic, 
so a good performance for simulated data should be demanded anyway." <a href = "https://webgate.ec.europa.eu/fpfis/mwikis/essnetbigdata/images/5/56/WPI_A_framework_for_mobile_network_data_micro-simulation_2019_02_14.pdf">(Salgado, D. et al. 2018)</a>

Th code is written in C++. To build the application you will need a C++ comppiler compliant with C++11.
 
</p>

<b>How to build the application under Windows operating system><b>
<p>
<p>Prerequisites
1. MSYS2 environment. MSYS2 is a software distro and building platform for Windows and 
it can be downloaded from https://www.msys2.org/. Installing MSYS2 is straightforward, detailed instructions
are provided on the project's web page mentoned above.
After downloading the MSYS2 the path to the C++ compiler and make utility have to be added to your PATH variable. It is advisable to add these
paths in front of your current content of the PATH variable. Supossing that we installed MSYS2 in C:\msys64 folder, if you intend to use the
standard command promt application provided by Windowd your PATH should be updated like this: PATH = C:\msys64\mingw64\bin;C:\msys64\usr\bin;%PATH%
Instead, you can use the sheel provided by MSYS2 by running C:\msys64\mingw64.exe which has al required environment variables already set.

2.GEOS C++ library. GEOS is a C++ port of JTS - Java Topology Suite. JTS 
is an open-source library that provides an object model for Euclidean planar linear geometry 
and is intended to be used as a core component of vector-based geographical information systems. GEOS contains a subset of
functions from JTS. The current version of the micro-simulator is developed using GEOS ver. 3.7.1. The source code can be downloaded from
https://trac.osgeo.org/geos. Instructions how to build the GEOS C++ library are provided here: https://trac.osgeo.org/geos/wiki/BuildingOnUnixWithAutotools
Mainly, you have to open a shell by running C:\msys64\mingw64.exe then type
$ ./autogen.sh
$ ./configure
$ make
$ make install

 
