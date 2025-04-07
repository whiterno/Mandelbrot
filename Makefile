CXX 	  := g++
CXX_STD   := -std=c++20
CXX_FLAGS := -O3

FLAG_ALGO 	  := 0
DOT_TYPE      := float
PARALLEL_COEF := 4

SFML := $(shell pkg-config --libs sfml-all --cflags)

INCLUDE_DIR := include

SRC := src/main.cpp src/tests.cpp src/draw_mandelbrot.cpp src/complex_nums.cpp src/app.cpp

OUT := main.out

compile:
	${CXX} ${CXX_STD} ${CXX_FLAGS} -DALGO_OPTIMIZATION=${FLAG_ALGO} -DDOT_TYPE=${DOT_TYPE} \
	-DPARALLEL_COEF=${PARALLEL_COEF} -I${INCLUDE_DIR} ${SFML} ${SRC} -o ${OUT}
