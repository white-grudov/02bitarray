#!/bin/bash

# Loop from n=1 to n=12
for n in {1..12}; do
    # Call valgrind with the appropriate command line arguments
    if [ $n -gt 6 ]
    then
        k=$((n-3))
    else
        k=$n
    fi

    if [ $n -le 3 ]
    then
        mode='-b'
    elif [ $n -le 6 ]
    then
        mode='-s'
    elif [ $n -le 9 ]
    then
        mode='-l'
    else
        mode='-i'
    fi

    echo $n
    ./main $mode < ./valgrind/data/$k >> /dev/null
done
