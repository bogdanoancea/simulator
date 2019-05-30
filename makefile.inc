#-------[ environment vars ]--------

PROJ_HOME = /Users/bogdanoancea/c-projects/simulator
GEOS_HOME = /Users/bogdanoancea/c-projects/geos-3.7.1
MSYS_HOME = C:/msys64/
DEBUG     = 0


#-------[ program names ]-----------

CXX    = g++.exe

#-------[ include path ]------------

INCS   = -I$(PROJ_HOME)/include
INCS  += -I$(GEOS_HOME)/include

#-------[ compiler flags ]----------

CXXFLAGS = $(INCS) -std=c++0x -O3 -pedantic -Wall -pedantic -Wall -DHAVE_CONFIG_H -DGEOS_INLINE -DUSE_UNSTABLE_GEOS_CPP_API -c -fmessage-length=0 -ffloat-store -Wno-long-long

ifeq ($(DEBUG),1)
  CXXFLAGS +=-DDEBUG -g3
endif


#------- linker flags ]------------

LDFLAGS  = -L$(GEOS_HOME)/src/.libs -L$(MSYS_HOME)/mingw64/lib
LIBS = $(GEOS_HOME)/src/.libs/libgeos.dll.a 


