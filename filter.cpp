#include "filter.h"
#include "waveform.h"

Filter::Filter(QObject *parent) : QObject(parent)
{

}

Waveform Filter::filtering(int ord, float *a, float *b, int np, float *x)
{
    std::vector<float> y(np, 0.f);
    int i,j;
    y[0]=b[0]*x[0];
    for (i=1;i<ord+1;i++)
    {
        y[i]=0.0;
        for (j=0;j<i+1;j++)
            y[i]=y[i]+b[j]*x[i-j];
        for (j=0;j<i;j++)
            y[i]=y[i]-a[j+1]*y[i-j-1];
    }
    /* end of initial part */
    for (i=ord+1;i<np+1;i++)
    {
        y[i]=0.0;
        for (j=0;j<ord+1;j++)
            y[i]=y[i]+b[j]*x[i-j];
        for (j=0;j<ord;j++)
            y[i]=y[i]-a[j+1]*y[i-j-1];
    }

    Waveform out("", y);
    return out;
}

Waveform Filter::filtering(const std::vector<float> &a, const std::vector<float> &b, const Waveform &wfm)
{
    if (a.size() !=  b.size())
        throw std::exception("filter parameters error");

    const float *x = wfm.data();
    int ord = a.size(), np = wfm.length();
    std::vector<float> y(np, 0.f);
    int i,j;
    y[0]=b[0]*x[0];
    for (i=1;i<ord+1;i++)
    {
        y[i]=0.0;
        for (j=0;j<i+1;j++)
            y[i]=y[i]+b[j]*x[i-j];
        for (j=0;j<i;j++)
            y[i]=y[i]-a[j+1]*y[i-j-1];
    }
    /* end of initial part */
    for (i=ord+1;i<np+1;i++)
    {
        y[i]=0.0;
        for (j=0;j<ord+1;j++)
            y[i]=y[i]+b[j]*x[i-j];
        for (j=0;j<ord;j++)
            y[i]=y[i]-a[j+1]*y[i-j-1];
    }

    Waveform out("", y);
    return out;
}
