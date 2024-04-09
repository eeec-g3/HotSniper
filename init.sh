#!/bin/bash

export PYTHONIOENCODING="UTF-8"
#setting $GRAPHITE_ROOT to HotSniper's root directory
export GRAPHITE_ROOT=$(pwd)
cd benchmarks
#setting $BENCHMARKS_ROOT to the benchmarks directory
export BENCHMARKS_ROOT=$(pwd)
#compiling the benchmarks
make
cd ..
