#!/bin/bash

# This script should be run if compiling on the JMU CS Cluster due to needing
# GCC 4.9.4 and having to pull igraph from a specific directory

export LD_LIBRARY_PATH=/shared/lib/igraph-0.7.1/build/lib:$LD_LIBRARY_PATH

source /shared/bin/gcc-4.9.4-setup.sh
