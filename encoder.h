#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include "waveform.h"

class IEncoder
{
public:
    virtual Waveform outputWithInputWfm(const Waveform &inputWfm) = 0;
};

class Encoder : public QObject, IEncoder
{
    Q_OBJECT

public:
    Encoder();
    virtual ~Encoder();
    virtual Waveform outputWithInputWfm(const Waveform &inputWfm);

private:
};

#endif // ENCODER_H
