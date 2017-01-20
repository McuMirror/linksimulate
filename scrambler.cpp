#include "scrambler.h"
#include "waveform.h"
#include <QVector>
#include <exception>

// http://wenku.baidu.com/view/4b5eac13f18583d04964596e.html

Scrambler::Scrambler(QObject *parent) : QObject(parent)
{
    m_eigenpolynomial = 0x8e;
    m_order = 8;
    m_initialPhase = 0x40;
}

Scrambler::Scrambler(char32_t eigenpolynomial, size_t order, char32_t initialPhase)
    : m_eigenpolynomial(eigenpolynomial)
    , m_order(order)
    , m_initialPhase(initialPhase)
{

}

Waveform Scrambler::scramblingCodeWithInput(const Waveform &wfm)
{
    if (m_eigenpolynomial >> (m_order - 1) != 0x1
            || m_eigenpolynomial >> m_order != 0x0
            || m_initialPhase >> m_order != 0x0)
        throw std::exception("Error - Scrambling parameter error, please check your input.");

    float *data = new float[wfm.length()]();
    char32_t registerStatus = m_initialPhase;
    for (int i = 0; i < wfm.length(); i++) {
        char32_t AND = registerStatus & m_eigenpolynomial;
        int count = 0;
        for (int j = 0; j < m_order; j++) {
            if ((AND >> j) % 2 == 1)
                count++;
        }
        int outputBit = (count % 2) ^ (int)wfm.data()[i];
        data[i] = outputBit;
        registerStatus = outputBit | (registerStatus << 1);
    }

    Waveform ret("", wfm.length(), data);
    delete [] data;

    return ret;
}

char32_t Scrambler::eigenpolynomial() const
{
    return m_eigenpolynomial;
}

void Scrambler::setEigenpolynomial(char32_t eigenpolynomial)
{
    m_eigenpolynomial = eigenpolynomial;
}

size_t Scrambler::order() const
{
    return m_order;
}

void Scrambler::setOrder(const size_t &order)
{
    m_order = order;
}

char32_t Scrambler::initialPhase() const
{
    return m_initialPhase;
}

void Scrambler::setInitialPhase(char32_t initialPhase)
{
    m_initialPhase = initialPhase;
}




