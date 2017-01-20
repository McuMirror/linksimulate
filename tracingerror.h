#ifndef TRACINGERROR_H
#define TRACINGERROR_H

#include <QObject>

class Waveform;

class TracingError : public QObject
{
    Q_OBJECT
public:
    explicit TracingError(QObject *parent = 0);
    static Waveform addTracingError(const Waveform &original, float errorValue);

signals:

public slots:
};

#endif // TRACINGERROR_H
