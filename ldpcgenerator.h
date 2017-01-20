#ifndef LDPCGENERATOR_H
#define LDPCGENERATOR_H

#include <QObject>
#include "encoder.h"

class Waveform;

class LDPCGenerator : public Encoder
{
    Q_OBJECT
public:
    explicit LDPCGenerator();
    Waveform outputWithInputWfm(const Waveform &inputWfm, int a);

signals:

public slots:
};

#endif // LDPCGENERATOR_H
