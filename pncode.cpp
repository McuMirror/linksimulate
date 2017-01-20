#include "pncode.h"
#include <exception>

#include <QDebug>

PNCode::PNCode(const char *name, size_t order, char32_t eigenpolynomial)
    : Waveform(name, (size_t)(pow(2.0f, (int)order) - 1))
    , m_eigenpolynomial(eigenpolynomial)
    , m_order(order)
{
    generator(order, eigenpolynomial);
}

void PNCode::generator(size_t order, char32_t eigenpolynomial)
{
    size_t shift = order - 1;
    if(eigenpolynomial >> order != 0x0
            || eigenpolynomial >> shift != 0x1
            || order > 32)
        throw std::exception("Error - PN code generating parameter error, please check your input.");

    char32_t regStatus = 0x1 << shift;       // Initialize register's status

    size_t l = length();
    float *values = new float[l]();
    for(size_t i = 0; i < l; i++) {
        values[i] = (float)((regStatus >> shift) % 2);

        char32_t temp = regStatus & eigenpolynomial;
        size_t numOne = 0;
        for(size_t j = 0; j < order; j++) {
            if((temp >> j) % 2 == 1)
                numOne++;
        }

        regStatus <<= 1;
        regStatus |= numOne % 2;
    }

    setData(values, 0, l);
    delete [] values;
}

