#! /bin/bash

script_dir=$(dirname "$(realpath "$0")")

cd "$script_dir" || exit

python3 -m venv venv
source venv/bin/activate

pip install matplotlib
pip install numpy

python build_plot.py

deactivate

rm -fr venv
