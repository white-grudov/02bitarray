#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <zip_file_name>"
  exit 1
fi

# Check if the necessary files and folder exist
if [ ! -d "include" ] || [ ! -f "main.cpp" ] || [ ! -f "Makefile" ]; then
  echo "Error: Required files and/or folder not found in the current directory."
  exit 1
fi

# Set the zip file name from the command-line argument
zip_file_name="../$1"

# Create the zip file in the parent folder
zip -FSr "$zip_file_name" include main.cpp Makefile >> /dev/null
