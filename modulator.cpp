#include "modulator.h"
#include <math.h>
#include <vector>

#include <QDebug>
#include "waveformmanager.h"
#include "frameconstructor.h"

#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.78539816339744830961

#define Frcos_Delay 4  // 12.7改过了，提供的经验值是 4
#define IF_SampleRate 4


Waveform Modulator::modulate(const Waveform &originWfm,
                             Modulator::Modulation modulation,
                             float IF)
{
    if (m_sampleRate == 0.f)
        m_sampleRate = originWfm.bitRate();        // MHz

    m_phaseNoise = INT_MIN;

    switch (modulation) {
    case PSK_B:
        return BPSK(originWfm, IF);
        break;
    case PSK_Q:
        return QPSK(originWfm, IF);
        break;
    case PSK_8:
        return PSK8(originWfm, IF);
        break;
    case APSK_16:
        return APSK16(originWfm, IF);
        break;
    case QAM_16:
        return QAM16(originWfm, IF);
        break;
    default:
        break;
    }
}

Waveform Modulator::BPSK(const Waveform &originWfm, float IF)
{
    // 映射
    std::vector<float> mod_sym_i, mod_sym_q;
    pskmod(originWfm, mod_sym_i, mod_sym_q, Modulator::PSK_B);

    // 插值
    float bitRate = originWfm.bitRate();
    std::vector<float> tempreg_i, tempreg_q;
    tempreg_i = interpolated(mod_sym_i, m_sampleRate, bitRate);
    tempreg_q = interpolated(mod_sym_q, m_sampleRate, bitRate);

    // 成形滤波
    float gain = 1;
    std::vector<float> samps_i, samps_q;
    samps_i = shapingFilter(tempreg_i, gain, m_sampleRate, bitRate, m_alpha);
    samps_q = shapingFilter(tempreg_q, gain, m_sampleRate, bitRate, m_alpha);

    // 上变频
    float sampleInterval = 1.f / m_sampleRate;
    //qDebug()<<sampleInterval;
    std::vector<float> data;
    data = upconverter(samps_i, samps_q, sampleInterval, m_DopplerShift, m_phaseNoise, IF);

    Waveform outWfm("", data);
    return outWfm;
}

Waveform Modulator::QPSK(const Waveform &originWfm, float IF)
{

    // 映射
    std::vector<float> mod_sym_i, mod_sym_q;
    pskmod(originWfm, mod_sym_i, mod_sym_q, Modulator::PSK_Q);

    // 插值
    float bitRate = originWfm.bitRate();

    std::vector<float> tempreg_i, tempreg_q;
    tempreg_i = interpolated(mod_sym_i, m_sampleRate, bitRate / 2);
    tempreg_q = interpolated(mod_sym_q, m_sampleRate, bitRate / 2);

    // 成形滤波
    float gain = 1;
    std::vector<float> samps_i, samps_q;
    samps_i = shapingFilter(tempreg_i, gain, m_sampleRate, bitRate / 2, m_alpha);
    samps_q = shapingFilter(tempreg_q, gain, m_sampleRate, bitRate / 2, m_alpha);

    // 上变频
    float sampleInterval = 1.f / m_sampleRate;
    std::vector<float> data;
    data = upconverter(samps_i, samps_q, sampleInterval, m_DopplerShift, m_phaseNoise, IF);

    Waveform outWfm("", data);
    return outWfm;
}

Waveform Modulator::PSK8(const Waveform &originWfm, float IF)
{
    // 映射
    std::vector<float> mod_sym_i, mod_sym_q;
    pskmod(originWfm, mod_sym_i, mod_sym_q, Modulator::PSK_8);

    // 插值
    float bitRate = originWfm.bitRate();
    std::vector<float> tempreg_i, tempreg_q;
    tempreg_i = interpolated(mod_sym_i, m_sampleRate, bitRate / 3);
    tempreg_q = interpolated(mod_sym_q, m_sampleRate, bitRate / 3);

    // 成形滤波
    float gain = 1;
    std::vector<float> samps_i, samps_q;
    samps_i = shapingFilter(tempreg_i, gain, m_sampleRate, bitRate / 3, m_alpha);
    samps_q = shapingFilter(tempreg_q, gain, m_sampleRate, bitRate / 3, m_alpha);

    // 上变频
    float sampleInterval = 1.f / m_sampleRate;
    std::vector<float> data;
    data = upconverter(samps_i, samps_q, sampleInterval, m_DopplerShift, m_phaseNoise, IF);

    Waveform outWfm("", data);
    return outWfm;
}

Waveform Modulator::APSK16(const Waveform &originWfm, float IF)
{
    // 映射
    std::vector<float> mod_sym_i, mod_sym_q;
    pskmod(originWfm, mod_sym_i, mod_sym_q, Modulator::APSK_16);

    // 插值
    float bitRate = originWfm.bitRate();
    std::vector<float> tempreg_i, tempreg_q;
    tempreg_i = interpolated(mod_sym_i, m_sampleRate, bitRate / 4);
    tempreg_q = interpolated(mod_sym_q, m_sampleRate, bitRate / 4);

    // 成形滤波
    float gain = 1;
    std::vector<float> samps_i, samps_q;
    samps_i = shapingFilter(tempreg_i, gain, m_sampleRate, bitRate / 4, m_alpha);
    samps_q = shapingFilter(tempreg_q, gain, m_sampleRate, bitRate / 4, m_alpha);

    // 上变频
    float sampleInterval = 1.f / m_sampleRate;
    std::vector<float> data;
    data = upconverter(samps_i, samps_q, sampleInterval, m_DopplerShift, m_phaseNoise, IF);

    Waveform outWfm("", data);
    return outWfm;
}

Waveform Modulator::QAM16(const Waveform &originWfm, float IF)
{
    // 映射
    std::vector<float> mod_sym_i, mod_sym_q;
    qammod(originWfm, mod_sym_i, mod_sym_q, Modulator::QAM_16);

    // 插值
    float bitRate = originWfm.bitRate();
    std::vector<float> tempreg_i, tempreg_q;
    tempreg_i = interpolated(mod_sym_i, m_sampleRate, bitRate / 4);
    tempreg_q = interpolated(mod_sym_q, m_sampleRate, bitRate / 4);

    // 成形滤波
    float gain = 1;
    std::vector<float> samps_i, samps_q;
    samps_i = shapingFilter(tempreg_i, gain, m_sampleRate, bitRate / 4, m_alpha);
    samps_q = shapingFilter(tempreg_q, gain, m_sampleRate, bitRate / 4, m_alpha);

    // 上变频
    float sampleInterval = 1.f / m_sampleRate;
    std::vector<float> data;
    data = upconverter(samps_i, samps_q, sampleInterval, m_DopplerShift, m_phaseNoise, IF);

    Waveform outWfm("", data);
    return outWfm;
}

float Modulator::sampleRate() const
{
    return m_sampleRate;
}

void Modulator::setSampleRate(float sampleRate)
{
    m_sampleRate = sampleRate;
}


Modulator::Modulator()
{
    m_sampleRate = 0.f;
    m_alpha = 0.5f;
}

std::vector<float> Modulator::rootRaisedCosine(float gain, float sampling_freq,
                                            float symbol_rate, float alpha, int ntaps)
{
    ntaps |= 1;	// ensure that ntaps is odd

    double spb = sampling_freq/symbol_rate; // samples per symbol
    std::vector<float> taps(ntaps);
    double scale = 0;
    for(int i=0;i<ntaps;i++)
    {
        double x1,x2,x3,num,den;
        double xindx = i - ntaps/2;
        x1 = M_PI * xindx/spb;
        x2 = 4 * alpha * xindx / spb;
        x3 = x2*x2 - 1;//alpha==1时，x3==0
        if( fabs(x3) >= 0.000001 )  // Avoid Rounding errors...
        {
            if( i != ntaps/2 )
                num = cos((1+alpha)*x1) + sin((1-alpha)*x1)/(4*alpha*xindx/spb);
            else
                num = cos((1+alpha)*x1) + (1-alpha) * M_PI / (4*alpha);
            den = x3 * M_PI;
        }
        else
        {
            if(alpha==1)
            {
                taps[i] = -1;
                continue;
            }
            x3 = (1-alpha)*x1;
            x2 = (1+alpha)*x1;
            num = (sin(x2)*(1+alpha)*M_PI
                   - cos(x3)*((1-alpha)*M_PI*spb)/(4*alpha*xindx)
                   + sin(x3)*spb*spb/(4*alpha*xindx*xindx));
            den = -32 * M_PI * alpha * alpha * xindx/spb;
        }
        taps[i] = alpha * num / den;
        scale += taps[i];
    }

    for(int i=0;i<ntaps;i++)
        taps[i] = 1e4 * taps[i] * gain / scale;

    return taps;
}

void Modulator::qammod(const Waveform &sequence_1, std::vector<float> &mod_sym_i,
                       std::vector<float> &mod_sym_q, Modulation XQAM)
{

    size_t length_1= sequence_1.length();
     std::vector<float> sequenceData(length_1 * 2, 0);
         for (int i = 0; i < length_1; i++) {
             sequenceData[i] = sequence_1.data()[i];
             sequenceData[i + length_1] = sequence_1.data()[i];
         }
     Waveform sequence("",sequenceData);//拷贝成两遍 下边可以验证输出

    /* for(int i=0;i<sequence.length();i++)
     {
         qDebug()<<sequence.data()[i];
     }*/

    int xstar;
    double sqrt10 = 1 / sqrt(2.0f);

    if(XQAM == QAM_16)
    {
        size_t M = 4;
        size_t symbolLength = sequence.length() % M == 0 ? sequence.length() / M : sequence.length() / M + 1;
        for(size_t i = 0; i < symbolLength; i++)
        {
            int bit1 = M * i < sequence.length() ? sequence.data()[M * i] : 0;
            int bit2 = M * i + 1 < sequence.length() ? sequence.data()[M * i + 1] : 0;
            int bit3 = M * i + 2 < sequence.length() ? sequence.data()[M * i + 2] : 0;
            int bit4 = M * i + 3 < sequence.length() ? sequence.data()[M * i + 3] : 0;
            xstar = bit1 + 2 * bit2 + 4 * bit3 + 8 * bit4;
            switch(xstar)
            {
            case 0: // R2,pi*3/12
            {
                mod_sym_i.push_back(sqrt10);
                mod_sym_q.push_back(sqrt10);
                break;
            }
            case 1: // R2,-pi*3/12
            {
                mod_sym_i.push_back(sqrt10);
                mod_sym_q.push_back(3 * sqrt10);
                break;
            }
            case 2: // R2,pi*9/12
            {
                mod_sym_i.push_back(3 * sqrt10);
                mod_sym_q.push_back(sqrt10);
                break;
            }
            case 3: // R2,-pi*9/12
            {
                mod_sym_i.push_back(3 * sqrt10);
                mod_sym_q.push_back(3 * sqrt10);
                break;
            }
            case 4: // R2,pi*1/12
            {
                mod_sym_i.push_back(sqrt10);
                mod_sym_q.push_back(-sqrt10);
                break;
            }
            case 5: // R2,-pi*1/12
            {
                mod_sym_i.push_back(sqrt10);
                mod_sym_q.push_back(-3 * sqrt10);
                break;
            }
            case 6: // R2,pi*11/12
            {
                mod_sym_i.push_back(3 * sqrt10);
                mod_sym_q.push_back(-sqrt10);
                break;
            }
            case 7: // R2,-pi*11/12
            {
                mod_sym_i.push_back(3 * sqrt10);
                mod_sym_q.push_back(-3 * sqrt10);
                break;
            }
            case 8: // R2,pi*5/12
            {
                mod_sym_i.push_back(-sqrt10);
                mod_sym_q.push_back(sqrt10);
                break;
            }
            case 9: // R2,-pi*5/12
            {
                mod_sym_i.push_back(-sqrt10);
                mod_sym_q.push_back(3 * sqrt10);
                break;
            }
            case 10: // R2,pi*7/12
            {
                mod_sym_i.push_back(-3 * sqrt10);
                mod_sym_q.push_back(sqrt10);
                break;
            }
            case 11: // R2,-pi*7/12
            {
                mod_sym_i.push_back(-3 * sqrt10);
                mod_sym_q.push_back(3 * sqrt10);
                break;
            }
            case 12: // R1,pi*3/12
            {
                mod_sym_i.push_back(-sqrt10);
                mod_sym_q.push_back(-sqrt10);
                break;
            }
            case 13: // R1,-pi*3/12
            {
                mod_sym_i.push_back(-sqrt10);
                mod_sym_q.push_back(-3 * sqrt10);
                break;
            }
            case 14: // R1,pi*9/12
            {
                mod_sym_i.push_back(-3 * sqrt10);
                mod_sym_q.push_back(-sqrt10);
                break;
            }
            case 15: // R1,-pi*9/12
            {
                mod_sym_i.push_back(-3 * sqrt10);
                mod_sym_q.push_back(-3 * sqrt10);
                break;
            }

            default:
            {
                printf("modulation type error 1 !\n");
            }
            }
        }
    }

    else
    {
        printf("qam modulation type error 2 !\n");
        getchar();
    }
}

void Modulator::pskmod(const Waveform &sequence_1, std::vector<float> &mod_sym_i,
                       std::vector<float> &mod_sym_q, Modulation XPSK)
{

   size_t length_1= sequence_1.length();
    std::vector<float> sequenceData(length_1 * 2, 0);
        for (int i = 0; i < length_1; i++) {
            sequenceData[i] = sequence_1.data()[i];
            sequenceData[i + length_1] = sequence_1.data()[i];
        }
    Waveform sequence("",sequenceData);//拷贝成两遍 下边可以验证输出
   /* for(int i=0;i<sequence.length();i++)
    {
        qDebug()<<sequence.data()[i];
    }*/

    float sqrt2;
    float R1,R2;
    float cos1_12,cos3_12,cos5_12;
    float sin1_12,sin3_12,sin5_12;
    float cos1_8,sin1_8;

    sqrt2 = 1/sqrt(2.0f);

    //对于16apsk，当4*R1^2+12*R2^2=16时，信号的平均能量为1。外圆和内圆半径的比为r=R1/R2
    R1 = 0.3606f;
    R2 = 1.1358f;

    cos1_12 = cos(M_PI/12);
    cos3_12 = cos(M_PI/4);
    cos5_12 = cos(5*M_PI/12);
    sin1_12 = sin(M_PI/12);
    sin3_12 = sin(M_PI/4);
    sin5_12 = sin(5*M_PI/12);
    cos1_8 = cos(M_PI/8);
    sin1_8 = sin(M_PI/8);

    size_t M, symbolLength;
    int xstar;
    switch (XPSK) {
    case PSK_B: {
        M = 1;
        symbolLength = sequence.length() / M;
        for (size_t i = 0; i < symbolLength; i++) {
            mod_sym_i.push_back(1 - 2 * (int)sequence.data()[i]);//已验证
            mod_sym_q.push_back(0);
        }
      /*  for(int j=0;j<symbolLength;j++)
            {
                qDebug()<<"mod_sym_i"<<mod_sym_i[j];
            }*/
        break;
    }
    case PSK_Q: {
        M = 2;
        symbolLength = sequence.length() % M == 0 ? sequence.length() / M : sequence.length() / M + 1;
        //qDebug()<<symbolLength;
        for (size_t i = 0; i < symbolLength; i++) {
        //qDebug()<<i;
            int bit1 = M * i < sequence.length() ? sequence.data()[M * i] : 0;
            int bit2 = M * i + 1 < sequence.length() ? sequence.data()[M * i + 1] : 0;
            xstar = bit1 + 2 * bit2;//映射关系 8421映射
            switch(xstar) {
            case 0:
                mod_sym_i.push_back(sqrt2);
                mod_sym_q.push_back(sqrt2);
                break;
            case 1:
                mod_sym_i.push_back(sqrt2);
                mod_sym_q.push_back(-sqrt2);
                break;
            case 2:
                mod_sym_i.push_back(-sqrt2);
                mod_sym_q.push_back(sqrt2);
                break;
            case 3:
                mod_sym_i.push_back(-sqrt2);
                mod_sym_q.push_back(-sqrt2);
                break;
            default:
                printf("modulation type error 11 !\n");
            }          
        }
        break;
    }
    case PSK_8: {
        M = 3;
        symbolLength = sequence.length() % M == 0 ? sequence.length() / M : sequence.length() / M + 1;
        //qDebug()<<symbolLength;
        for (size_t i = 0; i < symbolLength; i++) {
           //qDebug()<<i;
            int bit1 = M * i < sequence.length() ? sequence.data()[M * i] : 0;
            int bit2 = M * i + 1 < sequence.length() ? sequence.data()[M * i + 1] : 0;
            int bit3 = M * i + 2 < sequence.length() ? sequence.data()[M * i + 2] : 0;
            xstar = 4 * bit1 + 2 * bit2 + bit3;//映射关系8421映射，与所给示例是一样的
            switch(xstar) {
            case 0:
                mod_sym_i.push_back(cos1_8);
                mod_sym_q.push_back(sin1_8);
                break;
            case 4:
                mod_sym_i.push_back(sin1_8);
                mod_sym_q.push_back(cos1_8);
                break;
            case 6:
                mod_sym_i.push_back(-sin1_8);
                mod_sym_q.push_back(cos1_8);
                break;
            case 2:
                mod_sym_i.push_back(-cos1_8);
                mod_sym_q.push_back(sin1_8);
                break;
            case 3:
                mod_sym_i.push_back(-cos1_8);
                mod_sym_q.push_back(-sin1_8);
                break;
            case 7:
                mod_sym_i.push_back(-sin1_8);
                mod_sym_q.push_back(-cos1_8);
                break;
            case 5:
                mod_sym_i.push_back(sin1_8);
                mod_sym_q.push_back(-cos1_8);
                break;
            case 1:
                mod_sym_i.push_back(cos1_8);
                mod_sym_q.push_back(-sin1_8);
                break;

            default:
                printf("modulation type error 1x !\n");
        }
        }
         break;
    }
    case APSK_16: {
        M = 4;
        symbolLength = sequence.length() % M == 0 ? sequence.length() / M : sequence.length() / M + 1;
     // qDebug()<<symbolLength;
        for (size_t i = 0; i < symbolLength; i++) {
      //   qDebug()<<i;
            int bit1 = M * i < sequence.length() ? sequence.data()[M * i] : 0;
            int bit2 = M * i + 1 < sequence.length() ? sequence.data()[M * i + 1] : 0;
            int bit3 = M * i + 2 < sequence.length() ? sequence.data()[M * i + 2] : 0;
            int bit4 = M * i + 3 < sequence.length() ? sequence.data()[M * i + 3] : 0;
            xstar = bit1 + 2 * bit2 + 4 * bit3 + 8 * bit4;//映射关系 8421映射
            switch(xstar) {
            case 0: // R2,pi*3/12
                mod_sym_i.push_back(R2*cos3_12);
                mod_sym_q.push_back(R2*sin3_12);
                break;
            case 1: // R2,-pi*3/12
                mod_sym_i.push_back(R2*cos3_12);
                mod_sym_q.push_back(-R2*sin3_12);
                break;
            case 2: // R2,pi*9/12
                mod_sym_i.push_back(-R2*cos3_12);
                mod_sym_q.push_back(R2*sin3_12);
                break;
            case 3: // R2,-pi*9/12
                mod_sym_i.push_back(-R2*cos3_12);
                mod_sym_q.push_back(-R2*sin3_12);
                break;
            case 4: // R2,pi*1/12
                mod_sym_i.push_back(R2*cos1_12);
                mod_sym_q.push_back(R2*sin1_12);
                break;
            case 5: // R2,-pi*1/12
                mod_sym_i.push_back(R2*cos1_12);
                mod_sym_q.push_back(-R2*sin1_12);
                break;
            case 6: // R2,pi*11/12
                mod_sym_i.push_back(-R2*cos1_12);
                mod_sym_q.push_back(R2*sin1_12);
                break;
            case 7: // R2,-pi*11/12
                mod_sym_i.push_back(-R2*cos1_12);
                mod_sym_q.push_back(-R2*sin1_12);
                break;
            case 8: // R2,pi*5/12
                mod_sym_i.push_back(R2*cos5_12);
                mod_sym_q.push_back(R2*sin5_12);
                break;
            case 9: // R2,-pi*5/12
                mod_sym_i.push_back(R2*cos5_12);
                mod_sym_q.push_back(-R2*sin5_12);
                break;
            case 10: // R2,pi*7/12
                mod_sym_i.push_back(-R2*cos5_12);
                mod_sym_q.push_back(R2*sin5_12);
                break;
            case 11: // R2,-pi*7/12
                mod_sym_i.push_back(-R2*cos5_12);
                mod_sym_q.push_back(-R2*sin5_12);
                break;
            case 12: // R1,pi*3/12
                mod_sym_i.push_back(R1*cos3_12);
                mod_sym_q.push_back(R1*sin3_12);
                break;
            case 13: // R1,-pi*3/12
                mod_sym_i.push_back(R1*cos3_12);
                mod_sym_q.push_back(-R1*sin3_12);
                break;
            case 14: // R1,pi*9/12
                mod_sym_i.push_back(-R1*cos3_12);
                mod_sym_q.push_back(R1*sin3_12);
                break;
            case 15: // R1,-pi*9/12
                mod_sym_i.push_back(-R1*cos3_12);
                mod_sym_q.push_back(-R1*sin3_12);
                break;

            default:
                printf("modulation type error 1 !\n");
                break;
            }
        }
    default:
        break;
    }

    }
}

std::vector<float> Modulator::interpolated(const std::vector<float> &original, float sampling_freq,
                                           float symbol_rate)
{
    size_t samplePerBit = (int)(sampling_freq / symbol_rate); //采样率除以码率

    std::vector<float> output(original.size() * samplePerBit, 0.f);
    for (int i = 0; i < original.size(); i++) {       
     output[i*samplePerBit] = original[i];
    }

    return output;
}

std::vector<float> Modulator::shapingFilter(const std::vector<float> original, float gain, float sampling_freq,
                                            float symbol_rate, float alpha)
{
    size_t samplePerSymbol = (int)(sampling_freq / symbol_rate);
    size_t symbolLength = original.size() / samplePerSymbol;
    size_t frcosLength = 2 * samplePerSymbol * Frcos_Delay + 1;
    std::vector<float> samps;
    std::vector<float> fcoe = rootRaisedCosine(gain, sampling_freq, symbol_rate, alpha, frcosLength);

    std::vector<float> fltreg(frcosLength, 0.f);
    for (size_t i = 0; i < symbolLength; i++) {
        for (size_t j = 0; j < samplePerSymbol; j++) {
            for (size_t m = 0; m < frcosLength - 1; m++)
                fltreg[m] = fltreg[m + 1];
            fltreg[frcosLength - 1] = original[i * samplePerSymbol + j];

            float sum_temp = 0.f;

            for (size_t k = 0; k < frcosLength; k++) {
                sum_temp = sum_temp + fltreg[k] * fcoe[k];
            }

            samps.push_back(sum_temp);
        }
    }


    int sampsLength=samps.size();
    std::vector<float> sampsCut(sampsLength/2,0.f);

    for(int i=0;i<sampsLength/2;i++)//两倍后的截取
    {
        sampsCut[i] = samps[sampsLength / 4 + i];
    }

    return sampsCut;
}

std::vector<float> Modulator::upconverter(const std::vector<float> &samps_i, const std::vector<float> &samps_q,
                                          float sampleInterval, float DopplerShift, float phaseNoise, float IF)
{
   //float freq = IF + DopplerShift / 1e6;
    double dIF=(double)IF;
    double dShift = (double)DopplerShift;
    double freq = dIF + dShift / 1e6;
    //qDebug()<<"freq"<<freq;
    size_t sampleLength = samps_i.size() < samps_q.size() ? samps_i.size() : samps_q.size();

    std::vector<float> output(sampleLength, 0);

    for (size_t i = 0; i < sampleLength; i++) {
        float phase = 0.f;
        if (phaseNoise != INT_MIN) {
            float factor = pow(10, phaseNoise / 10);
            phase = sqrt(4 * M_PI * factor) * gaussrand();
        }
       //注意I路 - Q路
        long double dfre= 2 *(long double) M_PI * freq  * (long double)i * (long double)sampleInterval;

        long double dphase = (long double) phase;
       output[i] = samps_i[i] * cosl(dfre + dphase)- samps_q[i] * sinl(dfre + dphase);
    }
    return output;
}

float Modulator::gaussrand()
{
    static float V1, V2, S;
    static int phase = 0;
    float X;

    if ( phase == 0 ) {
        do {
            float U1 = (float)rand() / RAND_MAX;
            float U2 = (float)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return X;
}

float Modulator::alpha() const
{
    return m_alpha;
}

void Modulator::setAlpha(float alpha)
{
    m_alpha = alpha;
}

float Modulator::phaseNoise() const
{
    return m_phaseNoise;
}

void Modulator::setPhaseNoise(float phaseNoise)
{
    m_phaseNoise = phaseNoise;
}

float Modulator::DopplerShift() const
{
    return m_DopplerShift;
}

void Modulator::setDopplerShift(float DopplerShift)
{
    m_DopplerShift = DopplerShift;
}
