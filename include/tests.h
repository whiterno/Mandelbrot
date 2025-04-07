#ifndef TESTS_H
#define TESTS_H

const int PLATO_MICROSEC = 10000000;

enum Optimization_t{
    NO_OPTIMIZATION         = 0,
    ARM_NEON_OPTIMIZATION   = 1,
    PARALLEL_OPTIMIZATION   = 2,
};

void runTests(int tests_amount, draw_t draw);

void reachTempPlatoOp(draw_t draw, sf::Vertex* video_memory, ScaleView view);

#endif
