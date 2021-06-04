#!/bin/bash

libDir=lib
binDir=bin
srcDir=src
logDir=log
common=../library


name=network

function debugBuild {
	g++ -std=c++17 -Wall -g -fsanitize=leak\
        -I ${common} -I ${libDir}\
        -o ${binDir}/${name}\
	    ${srcDir}/main-network.cpp
}

function build {
	g++ -std=c++17 -O3 -flto -march=native\
        -I ${common} -I ${libDir}\
        -o ${binDir}/${name}\
		${srcDir}/main-network.cpp
}

#* Compile the source files
# debugBuild
build

#* Run
./${binDir}/${name} >> ${logDir}/log.txt
rm ${binDir}/${name}
