SRC 		:= src/app.cpp src/complex_nums.cpp src/draw_mandelbrot.cpp src/main.cpp src/tests.cpp
INCLUDE_DIR := include

all:
	cmake --build build
	./build/bin/main

compile:
	gcc ${SRC} -I ${INCLUDE_DIR} -I  -o main.out
