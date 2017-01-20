#ifndef GROUPDELAY_H
#define GROUPDELAY_H

#include <QObject>
#include <vector>

class Waveform;

class GroupDelay : public QObject
{
    Q_OBJECT
public:
    explicit GroupDelay(QObject *parent = 0);
    static Waveform addGroupDelay(const std::vector<float> &factors, const Waveform &original);     // factors in dB

signals:

public slots:
};

#endif // GROUPDELAY_H
