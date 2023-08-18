# -*- Makefile -*-
all: SpectraAnalyzer.x
SpectraAnalyzer.x: Main.cpp functions.cpp functions.hpp
	g++ -std=c++11 -Wall Main.cpp functions.cpp functions.hpp -o SpectraAnalyzer.x
# clean up
clean:
	rm -f *.x *.o
# for mac 
# g++ -std=c++11 -Wall Main.cpp functions.cpp functions.hpp 
