#ifndef MODULATOR_H
#define MODULATOR_H


#include "waveform.h"
#include <vector>

class Modulator
{
public:

    typedef enum : int {
        PSK_B = 0,
        PSK_Q,
        PSK_8,
        APSK_16,
        QAM_16
    } Modulation;

    Modulator();
    Waveform modulate(const Waveform &originWfm,
                      Modulation modulation,
                      float IF);                // MHz

    float sampleRate() const;
    void setSampleRate(float sampleRate);

    float DopplerShift() const;
    void setDopplerShift(float DopplerShift);

    float phaseNoise() const;
    void setPhaseNoise(float phaseNoise);

    float alpha() const;
    void setAlpha(float alpha);

private:
    Waveform BPSK(const Waveform &originWfm,
                  float IF);
    Waveform QPSK(const Waveform &originWfm,
                  float IF);
    Waveform PSK8(const Waveform &originWfm,
                  float IF);
    Waveform APSK16(const Waveform &originWfm,
                    float IF);
    Waveform QAM16(const Waveform &originWfm,
                   float IF);

    std::vector<float> rootRaisedCosine(float gain, float sampling_freq,
                                        float symbol_rate, float alpha, int ntaps);
    void qammod(const Waveform &sequence_1, std::vector<float> &mod_sym_i, std::vector<float> &mod_sym_q, Modulation XQAM);
    void pskmod(const Waveform &sequence_1, std::vector<float> &mod_sym_i, std::vector<float> &mod_sym_q, Modulation XPSK);

    std::vector<float> interpolated(const std::vector<float> &original, float sampling_freq, float symbol_rate);

    std::vector<float> shapingFilter(const std::vector<float> original, float gain, float sampling_freq,
                                     float symbol_rate, float alpha);

    std::vector<float> upconverter(const std::vector<float> &samps_i, const std::vector<float> &samps_q,
                                   float sampleInterval, float DopplerShift, float phaseNoise, float IF);

    float gaussrand();

    float           m_sampleRate;                       // MHz
    float           m_alpha;
    float           m_DopplerShift;                     // Hz
    float           m_phaseNoise;                       // dB
};

#endif // MODULATOR_H
