#ifndef WAVEFORM_H
#define WAVEFORM_H

#define DEBUG


#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

class Waveform
{
public:
    Waveform() { /*std::cout << "you should use Waveform()" << std::endl;*/ }
    Waveform(const char *name, size_t length);
    Waveform(const char *name, size_t length, const float *values);
    Waveform(const Waveform &wfm);
    Waveform(const char *name, const std::vector<float> &data);
    ~Waveform()
    {
       if(m_data != NULL)
            delete [] m_data;
    }

    Waveform &operator =(const Waveform &rhs);

    void                    setName(const char *name) { m_name = name; }
    std::string             name() const { return m_name; }
    size_t                  length() const { return m_length; }
    size_t                  size() const { return m_length * sizeof(float); }
    void                    setDescription(const char *des) { m_description = des; }
    std::string             description() const { return m_description; }
    const float *           data() const { return m_data; }
    void                    setData(const float *values, size_t startIndex, size_t length);
    const unsigned char *   binblock() const { return (unsigned char *)m_data; }
    float                   bitRate() const;
    void                    setBitRate(float bitRate);
    float                   sampleRate() const;
    void                    setSampleRate(float sampleRate);


#ifdef DEBUG
    void printData()
    {
        std::cout << m_name << std::endl;
        float *p = m_data;
        for(size_t i = 0; i < m_length; i++) {
            std::cout << *p << " ";
            p++;
        }
        std::cout << std::endl;
    }

    void printBinblock() {
        std::cout << m_name << std::endl;
        unsigned char *p = (unsigned char *)m_data;
        for(size_t i = 0; i < this->size(); i++) {
            char str[128];
            sprintf_s(str, "%d ", *p);
            std::cout << str;
            p++;
        }
        std::cout << std::endl;
    }
#endif

private:
    void initialize();
    void swap(Waveform &rhs);

    size_t              m_length;               // The number of waveform pionts
    std::string         m_name;
    float *             m_data=NULL;
    std::string         m_description;
    float               m_interval;             // Time interval between sample points, in microsecond.
    float               m_bitRate;              // In Mbps.
    float               m_sampleRate;
};

#endif // WAVEFORM_H
