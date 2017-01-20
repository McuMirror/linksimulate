#ifndef POLARIZATIONINTERFERENCE_H
#define POLARIZATIONINTERFERENCE_H

#include <QObject>

class Waveform;

class PolarizationInterference : public QObject
{
    Q_OBJECT
public:
    explicit PolarizationInterference(QObject *parent = 0);
    static Waveform polarizationInterference(const Waveform &original, float gain, size_t delay);

signals:

public slots:
};

#endif // POLARIZATIONINTERFERENCE_H
