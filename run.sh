#! /bin/bash


strategy=$1
packet=$2
machine=$3


spg run ${machine} ./generate.sh ${strategy} ${packet}