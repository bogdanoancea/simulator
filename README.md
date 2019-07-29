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

The code is written in C++. To build the application you need a C++ compiler compliant with C++11.
 
</p>

<b>How to build the application under <I>Windows</I> operating system</b>
<p>
Building the simulator application on Unix-like systems (Linux, MacOS X, etc.) is straightforward but on Windows
this is not a very easy task. That's why I provide here some information about the necessary steps to follow when 
one wants to build the application on Windows.
 
<p>Prerequisites
<p>1. MSYS2 environment. MSYS2 is a software distribution and building platform for Windows and 
it can be freely downloaded from https://www.msys2.org/. Installing MSYS2 is straightforward, detailed instructions
are provided on the project's web page mentioned above.
<p>After downloading the MSYS2 installer, run it and select to installation directory. 

<p>2. GNU C++ compiler on Windows with MSYS2
While there are a variety of toolchains to build C++ applications on Windows operating system and the most natural choice 
for Windows seems to be Visual Studio IDE we wanted to avoid proprietary software and use only open source software. This was the main
reason why we chose GNU C++ compiler. Besides being open source, it is also available on most used platforms: Linux, MacOS X, Windows etc.
The windows port of the GNU C/C++ compiler is called mingw. There are open source IDEs that contain a mingw compiler like CodeBocks or
Qt Creator, but in this tutorial I will only show how to build the simulator application from the command line.
After you installed MSYS2 use the <I>pacman</I> package manager to update all the packages. In an MSYS2 shell run the following command:
<br>$pacman -Syu

<p>
 After updating all packages, use <I>pacman</I> again to install the toolchain and C/C++ compiler. Run the command below:
<br>$pacman -S base-devel gcc vim cmake

<I>vim</I> and <I>cmake</I> are optional but good to have them installed.
Then, update the system PATH variable by adding the path to the C++ compiler and <I>make</I> utility.

<p>In the following I assume the default location for MSYS2 which is C:\msys64 folder. You can update the PATH variable using the standard Control Panel application provided by Windows.
If you intend to use the command prompt, your PATH could be updated using the following command:
>setx path C:\msys64\mingw64\bin;C:\msys64\usr\bin;"%path%

Instead, you can use the shell provided by Mingw MSYS2 by running C:\msys64\mingw64.exe which has all required environment variables already set.

<p>3. GEOS C++ library. GEOS is a C++ port of JTS - Java Topology Suite. JTS 
is an open-source library that provides an object model for Euclidean planar linear geometry 
and is intended to be used as a core component of vector-based geographical information systems. GEOS contains a subset of
functions from JTS. 
<p>The current version of the micro-simulator is developed using GEOS ver. 3.7.1. The source code can be downloaded from
https://trac.osgeo.org/geos. Instructions how to build the GEOS C++ library are provided here: https://trac.osgeo.org/geos/wiki/BuildingOnUnixWithAutotools
Mainly, you have to open a shell by running C:\msys64\mingw64.exe, go to the folder where your source code is located and then type
<br>$ ./autogen.sh
<br>$ ./configure
<br>$ make
<br>$ make install

<P> Compilation of the GOES library could take several minutes, be patient!.

<B>Build and run the simulation software</B>
<p>Download the source code of the micro-simulator from the github repository : https://github.com/bogdanoancea/simulator
Assuming that you want to download the source code in D:\data-simulator folder, open a Git shell in this folder, and then type:
<br>$git clone https://github.com/bogdanoancea/simulator.git
<br>After the source code was downloaded, open <I>makefile.inc</I> with any text editor and change the values of the following 3 variables:
<br>PROJ_HOME
<br>GEOS_HOME
<br>MSYS_HOME

<p> PROJ_HOME should point to the folder where you downloaded the source code of the micro-simulator (for example D:\data-simulator\simulator), 
GEOS_HOME should point to
the folder where is your GEOS source code and MSYS_HOME should indicate the folder where is your MSYS2 development environment.
After changing these values, save the file, open an MSYS2 shell (run C:\msys64\mingw64.exe), go to the folder where is your micro-simulator 
source code and type:
<br>$ make
<br>$ make install

<p> The executable is copied under the <I>Release folder</I>. To run a simulation type the following:
<br>$simulator.exe -m map.wkt -s simulation.xml -a antennas.xml -p persons.xml 

<p> We provide sample map.wkt, simulation.xml, antennas.xml and persons.xml configuration files in the root folder of the simulator source code. 
The output file is prob.csv.

<p>
<b>How to build the application under <I>Linux</I> or <I>Mac OS X</I> operating systems</b>
<p>1. Download GEOS C++ library from https://trac.osgeo.org/geos. Build the library by typing:
<br>$ ./autogen.sh
<br>$ ./configure
<br>$ make
<br>$ make install

<p>2. Download the source code of the micro-simulator from the github repository : https://github.com/bogdanoancea/simulator :
<br>$git clone https://github.com/bogdanoancea/simulator.git

<br>After the source code was downloaded, open <I>makefile.inc</I> with any text editor and change the values of 
the following 2 variables:
<br>PROJ_HOME
<br>GEOS_HOME

<p> PROJ_HOME should point to the folder where you downloaded the source code of the micro-simulator 
and GEOS_HOME should point to
the folder where is your GEOS source code. 
After changing these values, go to the directory where your micro-simulator source code is located and type the following commands in
a shell:
<br>$ make
<br>$ make install

<p>
<b>How to run the simulator</b>
<p>
<br>In a terminal (command prompt) type:
<br>$simulator -m map.wkt -s simulation.xml -a antennas.xml -p persons.xml -pb probabilities.xml -v -o
<br>
<br>where the following files provide input parameters:
<br> map.wkt is the map of the simulation
<br> simulation.xml is a configuration file containing the parameters of a simulation
<br> antennas.xml is the antennas configuration file
<br> persons.xml is the population configuration file
<br> probabilities.xml is the file containing the parameters need to compute the location probabilities
<br>
<br>The location probabilities are computed only if -o parameter is given in the command line. If -v is given in the command line the simulator will output on console the set of persons, mobile phone operators, antennas and mobile phones.


