#include "groupdelay.h"
#include "waveform.h"
#include "filter.h"

GroupDelay::GroupDelay(QObject *parent) : QObject(parent)
{

}

Waveform GroupDelay::addGroupDelay(const std::vector<float> &factors, const Waveform &original)
{
    std::vector<float> b(factors.size(), 0.f);
    for (size_t i = 0; i < factors.size(); i++) {
        float factor = pow(10, factors[i] / 10);
        b[i] = factor;
    }

    std::vector<float> a(b.size(), 0);
    a[0] = 1.f;

    return Filter::filtering(a, b, original);
}
