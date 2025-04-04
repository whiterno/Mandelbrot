#include "complex_nums.h"

Complex_t addC(Complex_t c1, Complex_t c2){
    Complex_t res = {.real = c1.real + c2.real, .imag = c1.imag + c2.imag};

    return res;
}

Complex_t subC(Complex_t c1, Complex_t c2){
    Complex_t res = {.real = c1.real - c2.real, .imag = c1.imag - c2.imag};

    return res;
}

Complex_t mulC(Complex_t c1, Complex_t c2){
    Complex_t res = {.real = c1.real * c2.real - c1.imag * c2.imag,
                     .imag = c1.imag * c2.real + c2.imag * c1.real};

    return res;
}

double modC(Complex_t c1){
    return c1.real * c1.real + c1.imag * c1.imag;
}
