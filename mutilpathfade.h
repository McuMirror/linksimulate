#ifndef MUTILPATHFADE_H
#define MUTILPATHFADE_H

#include <QObject>
#include <vector>
class Waveform;

class MultipathFade : public QObject
{
    Q_OBJECT
public:
    explicit MultipathFade(QObject *parent = 0);
    static Waveform multipathFadeWithGainAndDelay(const Waveform &originalWfm,
                                                  const std::vector<float> &gains,
                                                  const std::vector<float> &delaysInus);

signals:

public slots:
};

#endif // MUTILPATHFADE_H
