#include "tracingerror.h"
#include "waveform.h"

TracingError::TracingError(QObject *parent) : QObject(parent)
{

}

Waveform TracingError::addTracingError(const Waveform &original, float errorValue)
{
    std::vector<float> data(original.length(), 0.f);

    for (size_t i = 0; i < original.length(); i++) {
       data[i]=original.data()[i] * errorValue;
    }

    Waveform out("", data);

    return out;
}
