#ifndef RSCCGENERATOR_H
#define RSCCGENERATOR_H

#include "encoder.h"

// example is from http://wenku.baidu.com/view/5e257b1bb7360b4c2e3f64d1.html?re=view

class RSCCGenerator : public Encoder
{
public:
    RSCCGenerator();
    RSCCGenerator(const size_t n, const size_t k, const size_t m,const size_t rsccType,const size_t rsccPhase, const char16_t g[]);
    ~RSCCGenerator();
    Waveform outputWithInputWfm(const Waveform &inputWfm1);

    void                        setN(size_t n) { m_n = n; }
    void                        setK(size_t k) { m_k = k; }
    void                        setM(size_t m) { m_m = m; }
    void                        setG(char16_t g[], size_t cnt);

private:
    size_t                      m_n;                      // number of output
    size_t                      m_k;                      // number of serial to parallel
    size_t                      m_m;                      // number of register
    char16_t *                  m_g;                      // array of generation polynomials
    size_t                      m_rscctype;
    size_t                      m_rsccphase;

};

#endif // RSCCGENERATOR_H
