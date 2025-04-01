#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h>

struct Complex{
    double real;
    double imag;
};

Complex addC(Complex c1, Complex c2){
    Complex res = {.real = c1.real + c2.real, .imag = c1.imag + c2.imag};

    return res;
}

Complex subC(Complex c1, Complex c2){
    Complex res = {.real = c1.real - c2.real, .imag = c1.imag - c2.imag};

    return res;
}

Complex mulC(Complex c1, Complex c2){
    Complex res = {.real = c1.real * c2.real - c1.imag * c2.imag,
                   .imag = c1.imag * c2.real + c2.imag * c1.real};

    return res;
}

float modC(Complex c1){
    return c1.real * c1.real + c1.imag * c1.imag;
}

#endif
