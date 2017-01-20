#ifndef WAVEFORMMANAGER_H
#define WAVEFORMMANAGER_H


#include "waveformlist.h"
#include <unordered_map>
#include <QObject>

typedef std::tr1::unordered_map<std::string, WaveformPtr> WaveformMap;

class WaveformManager : public QObject
{
    Q_OBJECT

public:
    static WaveformManager *                        getInstance() { return m_instance; }

/**
 * <NOTE>
 * brief: The data of MAT-file should be double type.
 * author: Lonmoon
 * date: 2016/3/16
 */
    WaveformList                                    loadFromMAT(std::string &filePath);
    void                                            saveToMAT(const WaveformList &list, std::string &filePath);
    void                                            addWaveform(const Waveform &wfm);
    void                                            removeWaveformWithName(std::string name);
    const Waveform &                                waveformWithName(std::string name);
    const WaveformMap &                             sharedWaveform() const { return m_sharedWaveforms; }

signals:
    void dataChanged();

private:
    WaveformManager(QObject *parent = 0);
    WaveformManager(const WaveformManager &);
    WaveformManager & operator=(const WaveformManager &);

    static WaveformManager *                        m_instance;
    WaveformMap                                     m_sharedWaveforms;
};

#endif // WAVEFORMMANAGER_H
