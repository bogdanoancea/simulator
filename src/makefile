include ../makefile.inc


OBJECTS = agent/Agent.o\
agent/AgentsCollection.o\
SimException.o\
agent/Antenna.o\
AntennaInfo.o\
CSVParser.o\
Clock.o\
Constants.o\
Displace.o\
Distribution.o\
events/Event.o\
events/EventConfig.o\
events/CellIDEventConfig.o\
events/CellIDTAEventConfig.o\
events/CellIDEvent.o\
events/CellIDTAEvent.o\
events/EventFactory.o\
parsers/HomeWorkLocation.o\
parsers/HomeWorkScenario.o\
parsers/ConfigParser.o\
parsers/SimConfigParser.o\
parsers/PersonsConfigParser.o\
parsers/AntennaConfiguration.o\
parsers/AntennaConfigParser.o\
parsers/ProbabilitiesConfigParser.o\
RandomWalkDisplacement.o\
RandomWalkDriftDisplacement.o\
LevyFlightDisplacement.o\
HomeWorkDisplacement.o\
AgeDistribution.o\
UniformAgeDistribution.o\
TruncatedNormalAgeDistribution.o\
PostLocProb.o\
NetPriorPostLocProb.o\
UnifPriorPostLocProb.o\
agent/MobileOperator.o\
EMField.o\
map/Grid.o\
agent/HoldableAgent.o\
IDGenerator.o\
agent/ImmovableAgent.o\
InputParser.o\
agent/LocatableAgent.o\
map/Map.o\
map/WKTMap.o\
agent/MobilePhone.o\
agent/MovableAgent.o\
agent/Person.o\
RandomNumberGenerator.o\
agent/Tablet.o\
Tinyxml2.o\
Utils.o\
World.o\
main.o



$(TARGET): $(OBJECTS) 
	$(CXX) $(LDFLAGS) -o $@  $(OBJECTS)  $(LIBS)


install: $(TARGET)
	mkdir $(PROJ_HOME)/Release; mv $(TARGET) $(PROJ_HOME)/Release


.PHONY: clean
clean:
	rm -f *.o 
	rm -f *.d

.PHONY: cleanall
cleanall:
	rm -f *.o 
	rm -f *.exe
	rm -R $(PROJ_HOME)/Release/*
	rmdir $(PROJ_HOME)/Release

.PHONY: clbk
clbk:
	rm -f *~


.PHONY: dos2unix
dos2unix:
	dos2unix *

.PHONY: unix2dos
unix2dos:
	unix2dos *