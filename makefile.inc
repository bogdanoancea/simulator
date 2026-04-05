#-------[ environment vars ]--------

PROJ_HOME = /D/c-projects/simulator
GEOS_HOME = /usr/lib
DEBUG     = 0

#-------[TARGET, FLAGS etc depending on OS]---------

ifeq ($(OS),Windows_NT)
	CXX    = g++.exe
	TARGET = simulator.exe
	LDFLAGS =
	LIBS = -Wl,--push-state,-Bstatic $(GEOS_HOME)/libgeos.a -Wl,--pop-state
	CXXFLAGS += -march=native
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CCFLAGS += -D LINUX
		CXX = g++
		TARGET = simulator
		LDFLAGS =
		LIBS = $(GEOS_HOME)/libgeos.a
		CXXFLAGS += -march=native
	endif
	ifeq ($(UNAME_S),Darwin)
		CCFLAGS += -D OSX
		CXX = g++
		TARGET = simulator
		LDFLAGS = -L$(GEOS_HOME)
		LIBS = $(GEOS_HOME)/libgeos.dylib -lstdc++
		CXXFLAGS += -march=native
		CLANG_CXXFLAGS += -march=native
	endif
endif

ifeq ($(CXX),g++)
	CXXFLAGS += $(GCC_CXXFLAGS)
else ifeq ($(CXX),clang)
	CXXFLAGS += $(CLANG_CXXFLAGS)
else
	CXXFLAGS += $(UNKNOWN_CXXFLAGS)
endif


#-------[ include path ]------------

INCS   = -I$(PROJ_HOME)/include/
INCS  += -I$(PROJ_HOME)/include/agent
INCS  += -I$(PROJ_HOME)/include/map
INCS  += -I$(PROJ_HOME)/include/events
INCS  += -I$(PROJ_HOME)/include/parsers
INCS  += -I$(GEOS_HOME)/include

#-------[ compiler flags ]----------

CXXFLAGS += $(INCS) -std=c++17 -O3 -pedantic -Wall -DHAVE_CONFIG_H -DGEOS_INLINE -DUSE_UNSTABLE_GEOS_CPP_API -c -fmessage-length=0 -Wno-long-long

ifeq ($(DEBUG),1)
  CXXFLAGS +=-DDEBUG -g3
endif




