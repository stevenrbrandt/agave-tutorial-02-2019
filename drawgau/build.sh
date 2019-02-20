#!/bin/bash
set -x
cd ~/drawgau
mpicxx -std=c++11 -o drawgau drawgau.cpp