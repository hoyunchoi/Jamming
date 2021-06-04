#!/bin/bash

libDir=lib
binDir=bin
common=../library
strategy=$1
packet=$2

name=S${strategy}P${packet}

function debugBuild {
	g++ -std=c++17 -Wall -g -fsanitize=leak -I ${common} -I ${libDir}\
	    main.cpp\
        -o ${binDir}/${name}
}

function build {
	g++ -std=c++17 -O3 -flto -march=native -I ${common} -I ${libDir}\
		main.cpp\
        -o ${binDir}/${name}
}

#* Compile the source files
# debugBuild
build

#* Run
./${binDir}/${name} ${strategy} ${packet} >> log.txt
rm ${binDir}/${name}
