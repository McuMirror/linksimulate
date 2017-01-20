#include "waveform.h"
#include <time.h>
#include <exception>
#include <cstring>

Waveform::Waveform(const char *name, size_t length)
    : m_name(name)
    , m_length(length)
{
    initialize();
}

Waveform::Waveform(const char *name, size_t length, const float *values)
    : m_name(name)
    , m_length(length)
{
    initialize();
    setData(values, 0, length);
}

Waveform::Waveform(const Waveform &wfm)
    : m_name(wfm.name())
    , m_length(wfm.length())
{
    initialize();
    setData(wfm.data(), 0, wfm.length());
    m_bitRate = wfm.bitRate();
}

Waveform::Waveform(const char *name, const std::vector<float> &data)
    : m_name(name)
    , m_length(data.size())
{
    initialize();
    setData(data.data(), 0, data.size());
}

Waveform &Waveform::operator=(const Waveform &rhs)
{
    m_length = rhs.length();
    m_name = rhs.name();
    m_description = rhs.description();
    m_bitRate = rhs.bitRate();

//    float *originData = m_data;
    float *newData = new float[m_length]();
    for(int i = 0; i < rhs.length(); i++)
        newData[i] = rhs.data()[i];
    m_data = newData;

//    delete [] originData;

    return *this;
}

void Waveform::setData(const float *values, size_t startIndex, size_t length)
{
    if(startIndex < m_length && startIndex + length <= m_length) {
        for(size_t i = 0; i < length; i++) {
            m_data[startIndex + i] = values[i];
        }
    }
    else
        throw std::exception("Error - The range that you want to set is invalid");
}

void Waveform::initialize()
{
    if(m_name.empty()) {
        time_t t = time(0);
        char timeStr[64];
        tm timeInfo;
        localtime_s(&timeInfo, &t);
        strftime(timeStr, sizeof(timeStr), "Untitled_%Y%m%d%H%M%S", &timeInfo);
        m_name.assign(timeStr);
    }

    m_data = new float[m_length]();
    m_description = "null";
    m_interval = 1;
}

void Waveform::swap(Waveform &rhs)
{
    m_length = rhs.length();
    m_name = rhs.name();
    m_description = rhs.description();
    m_bitRate = rhs.bitRate();
}

float Waveform::bitRate() const
{
    return m_bitRate;
}

void Waveform::setBitRate(float bitRate)
{
    m_bitRate = bitRate;
}

float Waveform::sampleRate() const
{
    return m_sampleRate;
}

void Waveform::setSampleRate(float sampleRate)
{
    m_sampleRate = sampleRate;
}
