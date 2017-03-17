#! /bin/bash
dir1="."
. ./qhead.sh $0
#echo ====	$dir1	====
grep "$1" "$dir1"/*.c
grep "$1" "$dir1"/*.h
. ./qfoot.sh $0
