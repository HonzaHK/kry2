#!/bin/bash
FILE="doc"

make

rm $FILE.aux $FILE.dvi $FILE.ps $FILE.log $FILE.out $FILE.toc
