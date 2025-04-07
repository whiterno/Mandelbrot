#! /bin/bash

script_dir=$(dirname "$(realpath "$0")")

cd "$script_dir" || exit

DATA=tests/data

mkdir -p tests/data

make compile

./main.out --no-graphics-no-op --tests=100 > $DATA/no_op_100.txt
./main.out --no-graphics-arm --tests=100 > $DATA/arm_100.txt
./main.out --no-graphics-op --tests=100 > $DATA/op_100_4.txt

make compile PARALLEL_COEF=8

./main.out --no-graphics-op --tests=100 > $DATA/op_100_8.txt

make compile PARALLEL_COEF=12

./main.out --no-graphics-op --tests=100 > $DATA/op_100_12.txt

make compile PARALLEL_COEF=16

./main.out --no-graphics-op --tests=100 > $DATA/op_100_16.txt
