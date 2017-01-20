#ifndef PNCODE_H
#define PNCODE_H


#include "waveform.h"

class PNCode : public Waveform
{
public:
    /**
     * Example
     * Input:   order = 7, eigenpolynomial = 0x44 means D^7+D^3+1, 最低阶1省略, 馈线逻辑不输入
     * Output:  1000000100100110100
     *          1111011100001111111
     *          0001110110001010010
     *          1111101010100001011
     *          0111100111001010110
     *          0110000011011010111
     *          0100011001000
     */
    PNCode(const char *name, size_t order, char32_t eigenpolynomial);

    char16_t            eigenpolynomial() const { return m_eigenpolynomial; }
    size_t              order() const { return m_order; }

private:
    char32_t        m_eigenpolynomial;      // Eigenpolynomial represents the feedback logic used,
                                            //  Cn to C1, and n = m_Order.
    size_t          m_order;                // Order means the number of registers,
                                            //  and the max number is 32.
    void            generator(size_t order, char32_t eigenpolynomial);
};

#endif // PNCODE_H
