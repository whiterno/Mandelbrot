#! /bin/bash

script_dir=$(dirname "$(realpath "$0")")

cd "$script_dir" || exit

python3 build_plot.py

