include ../makefile.inc

TARGET  = simulator.exe

OBJECTS = Agent.o\
AgentsCollection.o\
Antenna.o\
AntennaInfo.o\
CSVParser.o\
Clock.o\
Constants.o\
EMField.o\
EventType.o\
Grid.o\
HoldableAgent.o\
IDGenerator.o\
ImmovableAgent.o\
InputParser.o\
LocatableAgent.o\
Map.o\
MobilePhone.o\
MovableAgent.o\
Person.o\
RandomNumberGenerator.o\
Tablet.o\
Tinyxml2.o\
Utils.o\
World.o\
main.o


$(TARGET): $(OBJECTS) 
	$(CXX) $(LDFLAGS) -o $@  $(OBJECTS)  $(LIBS)


install: $(TARGET)
	mkdir $(PROJ_HOME)/Release; cp $(TARGET) $(PROJ_HOME)/Release


.PHONY: clean
clean:
	rm -f *.o 
	rm -f *.d

.PHONY: cleanall
cleanall:
	rm -f *.o 
	rm -f *.exe
	rm -f $(TARGET)

.PHONY: clbk
clbk:
	rm -f *~


.PHONY: dos2unix
dos2unix:
	dos2unix *

.PHONY: unix2dos
unix2dos:
	unix2dos *