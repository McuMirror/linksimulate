#include "mutilpathfade.h"
#include "waveform.h"
#include "math.h"


#include <QDebug>

MultipathFade::MultipathFade(QObject *parent) : QObject(parent)
{

}

Waveform MultipathFade::multipathFadeWithGainAndDelay(const Waveform &originalWfm,
                                                      const std::vector<float> &gains,
                                                      const std::vector<float> &delaysInus)
{
    std::vector<float> gainsInAbs(gains.size(), 0.f);
    for (size_t i = 0; i < gains.size(); i++) {
        float gainInAbs = pow(10, gains[i] / 10);
        gainsInAbs[i] = gainInAbs;
    }

    std::vector<int> delays(delaysInus.size(), 0);
   float sampleInterval = 1 / originalWfm.sampleRate();
    for (size_t i = 0; i < delaysInus.size(); i++) {
        int delay = (int)(delaysInus[i] / sampleInterval);
        delays[i] = delay;
    }

    std::vector<float> mutilpathSum;
    int maxDelay = 0;
    for (int i = 0; i < delays.size(); i++)
        maxDelay = delays[i] > maxDelay ? delays[i] : maxDelay;
    size_t length = originalWfm.length() + maxDelay;

    for (size_t i = 0; i < length; i++) {
        // qDebug()<<i;
        float sum = 0.f;
        for (size_t j = 0; j < gainsInAbs.size(); j++) {
            float sampleValue = 0.f;
            if (i - delays[j] >= 0 && i - delays[j] < originalWfm.length())
                sampleValue = originalWfm.data()[i - delays[j]];
            else
                sampleValue = 0.f;
            sum += (gainsInAbs[j] * sampleValue);
        }
        mutilpathSum.push_back(sum);
    }
     qDebug()<<"OK";
    return Waveform("", mutilpathSum);
}
