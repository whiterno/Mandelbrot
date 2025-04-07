#! /bin/bash

script_dir=$(dirname "$(realpath "$0")")

cd "$script_dir" || exit

DATA=tests/data

mkdir -p $DATA

make compile

./main.out --no-graphics-no-op --tests=100 > $DATA/no_op_100.txt

echo ""
echo "-----------------------"
echo "No optimization tested!"
echo "-----------------------"
echo ""

./main.out --no-graphics-arm --tests=100 > $DATA/arm_100.txt

echo ""
echo "------------------------"
echo "ARM optimization tested!"
echo "------------------------"
echo ""

./main.out --no-graphics-op --tests=100 > $DATA/op_100_4.txt

echo ""
echo "------------------------------"
echo "x4 unroll optimization tested!"
echo "------------------------------"
echo ""

make compile UNROLL_COEF=8

./main.out --no-graphics-op --tests=100 > $DATA/op_100_8.txt

echo ""
echo "------------------------------"
echo "x8 unroll optimization tested!"
echo "------------------------------"
echo ""

make compile UNROLL_COEF=12

./main.out --no-graphics-op --tests=100 > $DATA/op_100_12.txt

echo ""
echo "-------------------------------"
echo "x12 unroll optimization tested!"
echo "-------------------------------"
echo ""

make compile UNROLL_COEF=16

./main.out --no-graphics-op --tests=100 > $DATA/op_100_16.txt

echo ""
echo "-------------------------------"
echo "x16 unroll optimization tested!"
echo "-------------------------------"
echo ""

make compile UNROLL_COEF=32

./main.out --no-graphics-op --tests=100 > $DATA/op_100_32.txt

echo ""
echo "-------------------------------"
echo "x32 unroll optimization tested!"
echo "-------------------------------"
echo ""
