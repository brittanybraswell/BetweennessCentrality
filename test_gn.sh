#!/bin/bash
#
#SBATCH --job-name=girvan-newman
#SBATCH --nodes=1
#SBATCH --ntasks=1

source ./setup.sh
make clean
make

echo ""
echo "-----Girvan-Newman-----"
echo "---------igraph--------"
echo ""
echo "edges.txt"
./girvan-newman bc/data/edges.txt igraph
echo ""
echo "dolphin.txt"
./girvan-newman bc/data/dolphin.txt igraph
echo ""
echo "collegefootball.txt"
./girvan-nawman bc/data/collegefootball.txt igraph
echo ""
echo "---------custom--------"
echo ""
echo "edges.txt"
./girvan-newman bc/data/edges.txt custom
echo ""
echo "dolphin.txt"
./girvan-newman bc/data/dolphin.txt custom
echo ""
echo "collegefootball.txt"
./girvan-newman bc/data/collegefootball.txt custom
