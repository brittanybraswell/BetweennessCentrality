#!/bin/bash

echo "Using Zachary's Karate Data Set"

TXT="data/edges.txt" 

echo "Vertex Betweenness"
echo "--Serial Version--"
OMP_NUM_THREADS=1 ./bc $TXT
