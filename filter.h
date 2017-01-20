#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <vector>

class Waveform;

class Filter : public QObject
{
    Q_OBJECT
public:
    explicit Filter(QObject *parent = 0);
    static Waveform filtering(int ord, float *a, float *b, int np, float *x);
    static Waveform filtering(const std::vector<float> &a, const std::vector<float> &b, const Waveform &wfm);

signals:

public slots:
};

#endif // FILTER_H
