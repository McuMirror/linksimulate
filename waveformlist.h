#ifndef WAVEFORMLIST_H
#define WAVEFORMLIST_H


#include "waveform.h"
#include <vector>
#include <memory>

typedef std::shared_ptr<Waveform>       WaveformPtr;
typedef std::vector<WaveformPtr>        WfmList;

class WaveformList
{
public:
    WaveformList(std::string name);
    WaveformList(const WaveformList &wfmList);
    ~WaveformList();

    WaveformList &      operator=(const WaveformList &rhs);
    std::string         name() const { return m_name; }
    void                setName(const char *name) { m_name = name; }
    size_t              count() const { return m_wfmList.size(); }
    void                addWaveform(const char *name, size_t length, const float *data);
    void                addWaveform(const Waveform &wfm);
    Waveform &          waveformWithIndex(size_t index);
    WfmList             waveformWithName(const char *name);
    const WfmList &     list() const { return m_wfmList; }

private:
    void initialize();

    std::string         m_name;
    WfmList             m_wfmList;
};

#endif // WAVEFORMLIST_H
