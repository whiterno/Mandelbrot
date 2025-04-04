#ifndef COMPLEX_NUMS_H
#define COMPLEX_NUMS_H

#include <math.h>

struct Complex_t{
    double real;
    double imag;
};

Complex_t addC(Complex_t c1, Complex_t c2);
Complex_t subC(Complex_t c1, Complex_t c2);
Complex_t mulC(Complex_t c1, Complex_t c2);
double modC(Complex_t c1);

#endif
