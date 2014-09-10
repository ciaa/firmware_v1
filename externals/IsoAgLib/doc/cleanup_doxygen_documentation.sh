#!/bin/sh

# A) make sure the current path is where this script lies.
cd `dirname $0`

# B) remove old DOC
rm -rf html/api/*
rm -rf html/all/*
rm doxywarnings.txt
