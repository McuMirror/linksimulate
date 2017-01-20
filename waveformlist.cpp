#include "waveformlist.h"
#include "time.h"
#include <cstdarg>

WaveformList::WaveformList(std::string name)
    : m_name(name)
{
    initialize();
}

WaveformList::WaveformList(const WaveformList &wfmList)
    : m_name(wfmList.name())
{
    this->m_wfmList = wfmList.list();
}

WaveformList::~WaveformList()
{
    // Because of the use of share_ptr, need not to use the destructor of Waveform class.
}

WaveformList &WaveformList::operator=(const WaveformList &rhs)
{
    // Have not remove the origin waveform list, memory leakage?
    this->setName(rhs.name().c_str());
    this->m_wfmList = rhs.list();
    return *this;
}

void WaveformList::addWaveform(const char *name, size_t length, const float *data)
{
    WaveformPtr wfm(new Waveform(name, length));
    wfm->setData(data, 0, length);
    m_wfmList.push_back(wfm);
}

void WaveformList::addWaveform(const Waveform &wfm)
{
    addWaveform(wfm.name().c_str(), wfm.length(), wfm.data());
}

Waveform &WaveformList::waveformWithIndex(size_t index)
{
    if(index >= this->list().size())
        throw std::exception("Error - The index selected is beyond the range of this list.");

    WaveformPtr pWfm = m_wfmList[index];
    return *(pWfm.get());
}

WfmList WaveformList::waveformWithName(const char *name)
{
    WfmList list;
    for(WfmList::const_iterator iter = this->list().begin()
        ; iter != this->list().end()
        ; iter++) {
        WaveformPtr wfm = *iter;
        if(strcmp(wfm.get()->name().c_str(), name))
            list.push_back(wfm);
    }

    return list;
}

void WaveformList::initialize()
{
    if(m_name.empty()) {
        time_t t = time(0);
        char timeStr[64];
        tm timeInfo;
        localtime_s(&timeInfo, &t);
        strftime(timeStr, sizeof(timeStr), "Untitled_%Y%m%d%H%M%S", &timeInfo);
        m_name.assign(timeStr);
    }
}
