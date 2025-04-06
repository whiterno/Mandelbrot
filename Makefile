CXX 	  := g++
CXX_STD   := -std=c++20
CXX_FLAGS := -O3

CXX_FLAG_ALGO := -DALGO_OPTIMIZATION=1
CXX_FLAG_TYPE := -Ddata_type=double

SFML := $(shell pkg-config --libs sfml-all --cflags)

INCLUDE_DIR := include
BUILD_DIR	:= build

SRC := src/main.cpp src/tests.cpp src/draw_mandelbrot.cpp src/complex_nums.cpp src/app.cpp

OUT := main.out

no_algo_op_float:
	${CXX} ${CXX_STD} ${CXX_FLAGS} -I${INCLUDE_DIR} ${SFML} ${SRC} -o ${OUT}

algo_op_float:
	${CXX} ${CXX_STD} ${CXX_FLAGS} ${CXX_FLAG_ALGO} -I${INCLUDE_DIR} ${SFML} ${SRC} -o ${OUT}

no_algo_op_double:
	${CXX} ${CXX_STD} ${CXX_FLAGS} ${CXX_FLAG_TYPE} -I${INCLUDE_DIR} ${SFML} ${SRC} -o ${OUT}

algo_op_double:
	${CXX} ${CXX_STD} ${CXX_FLAGS} ${CXX_FLAG_ALGO} ${CXX_FLAG_TYPE} -I${INCLUDE_DIR} ${SFML} ${SRC} -o ${OUT}
