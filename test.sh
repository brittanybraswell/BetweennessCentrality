#!/bin/bash
#
#SBATCH --job-name=final-proj
#SBATCH --nodes=1
#SBATCH --ntasks=1

labels=("Zachary's Karate Club" "Dolphin Network" "Les Miserables Network")
texts=("data/edges.txt" "data/dolphin.txt" "data/lesmis.txt")
files=("vertex_bt")
index=0

make clean -C bc
make -C bc

echo ""
echo "-----FIND IMPORTANT VERTEX-----"
echo ""
for text in ${texts[@]}
do
    echo ${labels[$index]}
    for file in ${files[@]}
    do
        echo "./"$file
        for i in 1 2 4 8 16
        do
            printf "Thread Count: %d\t" "$i"
            OMP_NUM_THREADS=${i} ./bc/$file bc/$text
        done
    done
    index=$((index+1))
    echo ""
done

echo "-----PERFORMANCE TESTING-----"
echo ""
labels=("College Football" "Scientific Collaboration Network" "Facebook Network")
texts=("data/collegefootball.txt" "data/collab.txt" "data/fb.txt")
index=0
for text in ${texts[@]}
do
    echo ${labels[$index]}
    for file in ${files[@]}
    do
        echo "./"$file
        for i in 1 4 8 16
        do
            printf "Thread Count: %d\t" "$i"
            OMP_NUM_THREADS=${i} ./bc/$file bc/$text
        done
    done
    index=$((index+1))
    echo ""
done
