CXX 	  := g++
CXX_STD   := -std=c++20
CXX_FLAGS := -O3

FLAG_ALGO 	?= 0
DOT_TYPE    ?= float
UNROLL_COEF ?= 4

SFML_LIBS  := $(shell pkg-config --libs sfml-all)
SFML_FLAGS := $(shell pkg-config --cflags sfml-all)

INCLUDE_DIR := include

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, obj/%.o, ${SRC})
INC := $(wildcard include/*.h)

OUT := main.out

VAR_FLAG := obj/.compile_flags

$(shell mkdir -p obj)

compile: ${VAR_FLAG}.tmp ${OBJ} ${INC}
	${CXX} ${CXX_STD} ${CXX_FLAGS} -I${INCLUDE_DIR} ${SFML_LIBS} ${OBJ} -o ${OUT}

${VAR_FLAG}.tmp:
	@echo "${FLAG_ALGO} ${DOT_TYPE} ${UNROLL_COEF}" > $@
	@cmp -s $@ ${VAR_FLAG} || mv $@ ${VAR_FLAG}
	@rm -f $@

obj/%.o: src/%.cpp ${INC} ${VAR_FLAG}
	${CXX} -c ${CXX_STD} ${CXX_FLAGS} -DALGO_OPTIMIZATION=${FLAG_ALGO} -DDOT_TYPE=${DOT_TYPE} \
	-DUNROLL_COEF=${UNROLL_COEF} -I${INCLUDE_DIR} ${SFML_FLAGS} $< -o $@
