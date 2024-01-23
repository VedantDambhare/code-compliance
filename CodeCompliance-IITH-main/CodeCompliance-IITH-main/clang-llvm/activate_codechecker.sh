#!/bin/bash

src="$1"
input_path="$2"

source $src
export PATH=~/codechecker/build/CodeChecker/bin:$PATH

CodeChecker check -b "gcc $input_path" --output ./results --analyzers clang-tidy -e misc-passes

CodeChecker parse -e html ./results -o ./reports_html