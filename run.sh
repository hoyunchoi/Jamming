#! /bin/bash


strategy=$1
packetRepeat=$2
packet=$3
machine=$4


spg run ${machine} ./generate.sh ${strategy} ${packet} ${packetRepeat}