#!/bin/bash

for input_file in input/*; do
  output_file="answers/$(basename "$input_file" .dat)_ans.dat"
  
  ./../build/tree < "$input_file" > temp_output.txt
  
  if diff -q "$output_file" temp_output.txt > /dev/null; then
    echo "PASS: $input_file"
  else
    echo "FAIL: $input_file"
  fi
  
  rm temp_output.txt
done