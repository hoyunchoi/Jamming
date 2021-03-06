#!/bin/bash

libDir=lib
binDir=bin
srcDir=src
logDir=log
common=../library

strategy=$1
packetRepeat=$2
packet=$3

name=S${strategy}P${packet}R${packetRepeat}

function debugBuild {
	g++ -std=c++17 -Wall -g -fsanitize=leak\
        -I ${common} -I ${libDir}\
        -o ${binDir}/${name}\
	    ${srcDir}/main-generate.cpp
}

function build {
	g++ -std=c++17 -O3 -flto -march=native\
        -I ${common} -I ${libDir}\
        -o ${binDir}/${name}\
		${srcDir}/main-generate.cpp
}

#* Compile the source files
# debugBuild
build

#* Run
./${binDir}/${name} ${strategy} ${packet} ${packetRepeat} >> ${logDir}/log.txt
rm ${binDir}/${name}
