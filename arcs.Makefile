all: build/testArcs

build/testArcs: arc.cpp arc.h simulate.h
	g++ -DSIMULATE -o build/testArcs arc.cpp