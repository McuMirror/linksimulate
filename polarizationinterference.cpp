#include "polarizationinterference.h"
#include "waveform.h"
#include "math.h"

PolarizationInterference::PolarizationInterference(QObject *parent) : QObject(parent)
{

}

Waveform PolarizationInterference::polarizationInterference(const Waveform &original, float gain, size_t delay)
{
    std::vector<float> data;
    size_t length = original.length() + delay;
    float gainInAbs = pow(10, gain / 10);

    for (size_t i = 0; i < length; i++) {
        float sum = 0.f;
        float polarA = i < delay ? 0.f : original.data()[i - delay];
        float originalA = i < original.length() ? original.data()[i] : 0.f;

        sum = originalA + polarA * gainInAbs;
        data.push_back(sum);
    }

    return Waveform("", data);
}
