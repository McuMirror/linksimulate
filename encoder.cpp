#include "encoder.h"
#include <exception>


Encoder::Encoder()
{
}

Encoder::~Encoder()
{

}

Waveform Encoder::outputWithInputWfm(const Waveform &inputWfm)
{
    return inputWfm;
}
