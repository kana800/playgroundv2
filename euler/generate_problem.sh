#!/bin/bash

if [ $1 ]; then
	echo """
#include <stdio.h>
	
int main(int argc, char **argv)
{
	return 0;
}
""" >> $1.c

	echo """
day$1: $1.c
	\$(CC) -g $^ -o \$@.o
""" >> Makefile
else
	echo "usage: bash generate_file.sh <problem#>"
fi
