#!/bin/sh
# Author: Shazeb Suhail
# Date: Saturday October 10th, 2023

if [ -z "$1" ]; then
  echo "ERROR! Please add a filename argument to execute this script properly."
else
  gcc $1.c -pthread -o $1.out
  if [ $? -eq 0 ]; then
    ./$1.out
  else
    echo "ERROR! Failed to compile filename: $1"
  fi
fi
