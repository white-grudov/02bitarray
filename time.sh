#!/bin/bash

# Set the number of runs (default is 10)
n=${1:-10}

# Define the commands to be executed and their corresponding input files
commands=(
  "./main -l < a22 >> /dev/null"
  "./main -l < a22_s >> /dev/null"
  "./main -p < a22 >> /dev/null"
  "./main -p < a22_s >> /dev/null"
)

# Loop through the commands and measure their execution time
for cmd in "${commands[@]}"
do
  total_time=0

  # Run the command n times and measure the total execution time
  for ((i=1; i<=n; i++))
  do
    start_time=$(date +%s.%N)
    eval $cmd
    end_time=$(date +%s.%N)
    total_time=$(echo "$total_time + $end_time - $start_time" | bc -l)
  done

  # Calculate the average execution time and print the result
  avg_time=$(echo "scale=3; $total_time / $n" | bc -l)
  echo "Average execution time of command '$cmd' over $n runs: $avg_time seconds"
done
