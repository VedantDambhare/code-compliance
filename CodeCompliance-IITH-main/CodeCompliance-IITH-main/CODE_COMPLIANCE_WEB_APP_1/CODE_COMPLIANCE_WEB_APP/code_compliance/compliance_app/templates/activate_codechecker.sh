#!/bin/bash

src="$1"
input_path="$2"

files_path=()

source $src
export PATH=~/codechecker/build/CodeChecker/bin:$PATH

for file in "$input_path"/*.cpp; do
    
    if [ -f "$file" ]; then
        # Append the file name to the variable, separating with a space       
        files_path+=("$(basename "$file")")
    fi
done;


file_path=""


for filename in "${files_path[@]}"; do
    # Construct the full file path
    file_path="${file_path}${input_path}/${filename} "
done

echo $file_path
CodeChecker check -b "g++ $file_path" --output ./results --analyzers clang-tidy --disable-all -e compliance

CodeChecker parse -e html ./results -o ./reports_html
