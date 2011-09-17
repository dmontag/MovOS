#!/bin/sh

if [ -z "$1" -o "$1" = "--help" -o "$1" = "-h" ]
then
	echo "usage: $0 <infile1> <infile2> <outfile>"
	exit 0
fi

if [ ! -e "$1" ]
then
	echo "$1 does not exist."
	exit 1
fi

if [ ! -e "$2" ]
then
	echo "$2 does not exist."
	exit 1
fi

if [ -z $3 ]
then
	echo "Target file does not exist."
	exit 1
fi
 
cp "$1" "$3"
cat "$2" >> "$3"

exit 0
