#-------[ environment vars ]--------

PROJ_HOME = c:/c-projects/simulator
GEOS_HOME = C:/msys64/mingw64/lib
MSYS_HOME = C:/msys64/
DEBUG     = 0


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
	LDFLAGS = -L$(MSYS_HOME)/mingw64/lib
	LIBS = $(GEOS_HOME)/libgeos.dll.a
	CXXFLAGS = -march=native

else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
		CXX		= g++
		TARGET  = simulator
		LDFLAGS = -L$(GEOS_HOME)
		LIBS = libgeos.a
		CXXFLAGS = -march=native

    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
		CXX		= g++
		TARGET	= simulator
		LDFLAGS = -L$(GEOS_HOME)
		LIBS = libgeos.a
		CXXFLAGS = -march=native
    endif
endif


#-------[ include path ]------------

INCS   = -I$(PROJ_HOME)/include
INCS += -I$(GEOS_HOME)/../include

#-------[ compiler flags ]----------

CXXFLAGS += $(INCS) -std=c++17 -O3 -mavx2 -pedantic -Wall -DHAVE_CONFIG_H -DGEOS_INLINE -DUSE_UNSTABLE_GEOS_CPP_API -c -fmessage-length=0 -Wno-long-long

ifeq ($(DEBUG),1)
  CXXFLAGS +=-DDEBUG -g3
endif




