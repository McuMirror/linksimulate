#include "normalizer.h"
#include "waveform.h"
#include <math.h>

Normalizer::Normalizer(QObject *parent) : QObject(parent)
{

}

Waveform Normalizer::normalizing(const Waveform &originalWfm)
{
    std::vector<float> data(originalWfm.length(), 0);
    float max = 0.f;

    for (size_t i = 0; i < originalWfm.length(); i++) {
        if (abs(originalWfm.data()[i]) > max)
            max = abs(originalWfm.data()[i]);
    }

    for (size_t i = 0; i < originalWfm.length(); i++) {
        data[i] = originalWfm.data()[i] / max;
    }

    Waveform wfm("", data);
    return wfm;
}
