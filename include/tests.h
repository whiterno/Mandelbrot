#ifndef TESTS_H
#define TESTS_H

enum Optimization_t{
    NO_OPTIMIZATION         = 0,
    ARM_NEON_OPTIMIZATION   = 1,
    PARALLEL_OPTIMIZATION   = 2,
};

double runTests(int tests_amount, int optimization);

#endif
