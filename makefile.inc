#-------[ environment vars ]--------

PROJ_HOME = D:/data-simulator/simulator/
DEBUG     = 0


#-------[ program names ]-----------

CXX    = g++.exe

#-------[ include path ]------------

INCS   = -I"D:/data-simulator/simulator/include" 
INCS  += -I"D:/data-simulator/geos-3.7.1/include"

#-------[ compiler flags ]----------

CXXFLAGS = $(INCS) -std=c++0x -O3 -pedantic -Wall -pedantic -Wall -DHAVE_CONFIG_H -DGEOS_INLINE -DUSE_UNSTABLE_GEOS_CPP_API -c -fmessage-length=0 -ffloat-store -Wno-long-long

ifeq ($(DEBUG),1)
  CXXFLAGS +=-DDEBUG -g3
endif


#------- linker flags ]------------

LDFLAGS  = -L"D:/data-simulator/geos-3.7.1/src/.libs" -L"C:/msys64/mingw64/lib" 
LIBS = D:/data-simulator/geos-3.7.1/src/.libs/libgeos.dll.a 


