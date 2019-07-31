#-------[ environment vars ]--------

PROJ_HOME = D:\data-simulator\simulator
GEOS_HOME = D:\data-simulator\geos-3.7.1
MSYS_HOME = C:/msys64/
DEBUG     = 0

CXXFLAGS = -Og -Wall -Wextra

GCC_CXXFLAGS = -DMESSAGE='"Compiled with GCC"'
CLANG_CXXFLAGS = -DMESSAGE='"Compiled with Clang"'
UNKNOWN_CXXFLAGS = -DMESSAGE='"Compiled with an unknown compiler"'

ifeq ($(CXX),g++)
  CXXFLAGS += $(GCC_CXXFLAGS)
else ifeq ($(CXX),clang)
  CXXFLAGS += $(CLANG_CXXFLAGS)
else
  CXXFLAGS += $(UNKNOWN_CXXFLAGS)
endif


#-------[TARGET, FLAGS etc depending on OS]---------

ifeq ($(OS),Windows_NT)
	CXX    = g++.exe
	TARGET = simulator.exe
	LDFLAGS  = -L$(GEOS_HOME)/src/.libs -L$(MSYS_HOME)/mingw64/lib 
	LIBS = $(GEOS_HOME)/src/.libs/libgeos.dll.a -lgomp
	CXXFLAGS = -march=native -D_GLIBCXX_PARALLEL -fopenmp
	
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
		CXX		= g++
		TARGET  = simulator
		LDFLAGS  = -L$(GEOS_HOME)/src/.libs 
		LIBS = $(GEOS_HOME)/src/.libs/libgeos.a -lgomp
		CXXFLAGS = -march=native -D_GLIBCXX_PARALLEL -fopenmp
		
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
		CXX		= g++
		TARGET	= simulator
		LDFLAGS  = -L$(GEOS_HOME)/src/.libs 
		LIBS = $(GEOS_HOME)/src/.libs/libgeos.a
    endif
endif


#-------[ include path ]------------

INCS   = -I$(PROJ_HOME)/include
INCS  += -I$(GEOS_HOME)/include

#-------[ compiler flags ]----------

CXXFLAGS += $(INCS) -std=c++17 -O3 -pedantic -Wall -DHAVE_CONFIG_H -DGEOS_INLINE -DUSE_UNSTABLE_GEOS_CPP_API -c -fmessage-length=0 -Wno-long-long 

ifeq ($(DEBUG),1)
  CXXFLAGS +=-DDEBUG -g3
endif




