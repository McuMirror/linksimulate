#ifndef NORMALIZER_H
#define NORMALIZER_H

#include <QObject>

class Waveform;

class Normalizer : public QObject
{
    Q_OBJECT
public:
    explicit Normalizer(QObject *parent = 0);
    Waveform normalizing(const Waveform &originalWfm);

signals:

public slots:
};

#endif // NORMALIZER_H
