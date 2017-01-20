#include "rsccgenerator.h"
#include<QDebug>


RSCCGenerator::RSCCGenerator()
{
    // Example
    m_n = 3;
    m_k = 2;
    m_m = 1;
    m_g = new char16_t[6]();
    char16_t arr[6] = { 3, 1, 1, 2, 3, 2 };
    for(int i = 0; i < 6; i++)
        m_g[i] = arr[i];
}

RSCCGenerator::RSCCGenerator(const size_t n, const size_t k, const size_t m, const size_t rsccType, const size_t rsccPhase, const char16_t g[])
    : m_n(n)
    , m_k(k)
    , m_m(m)
    , m_rscctype(rsccType)
    , m_rsccphase(rsccPhase)
{
    size_t numElem = n * k;
    m_g = new char16_t[numElem]();
    for(int i = 0; i < numElem; i++)
        *(m_g + i) = g[i];
}

RSCCGenerator::~RSCCGenerator()
{
    delete [] m_g;
}

Waveform RSCCGenerator::outputWithInputWfm(const Waveform &inputWfm1)
{
    qDebug()<<"卷积输入长度 "<<" "<<inputWfm1.length();
    size_t length = m_n * (inputWfm1.length() + m_m);

    //对输入做二倍复制处理，以保留相关性 2017.1.11日改

    size_t inLength= inputWfm1.length();

    std::vector<float> copyData(inLength* 2, 0);
         for (int i = 0; i < inLength; i++) {
             copyData[i] = inputWfm1.data()[i];
             copyData[i + inLength] = inputWfm1.data()[i];
         }
     Waveform inputWfm("",copyData); //拷贝成两遍 下边可以验证输出


    if(m_m >= 16)
        throw std::exception("Error - The number of registers should less then 16.");

    size_t L = inputWfm.length() % m_k == 0 ? inputWfm.length() / m_k
                                            : inputWfm.length() / m_k + 1;
    size_t lengthDouble = m_n * (L + m_m);
    float *dataDouble = new float[lengthDouble]();

    char16_t *regStatus = new char16_t[m_k];
    memset(regStatus, 0, m_k * sizeof(char16_t));

    for(size_t iOrigin = 0, iRSCC = 0
        ; iRSCC < lengthDouble
        ; iOrigin += m_k, iRSCC += m_n) {
        for(size_t j = 0; j < m_k; j++) {
            int inputValue = 0;
            if(iOrigin + j < inputWfm.length()) {
                float value = inputWfm.data()[iOrigin + j];
                inputValue = (int)value;
            }
            inputValue <<= m_m;

            regStatus[j] >>= 1;
            regStatus[j] |= inputValue;
        }

        for(size_t i = 0; i < m_n; i++) {
            size_t count = 0;

            for(size_t j = 0; j < m_k; j++) {
                char16_t temp = regStatus[j] & m_g[j + i * m_k];
                for(size_t p = 0; p <= m_m; p++) {
                    if((temp >> p) % 2 == 1)
                        count++;
                }
            }

            dataDouble[iRSCC + i] = (float)(count % 2);
        }
    }

    /*for(int i=0;i<length;i++) //验证双倍后正确
    {
        qDebug()<<"data1 "<<i<<" "<<data[i];
    }*/

   float *data = new float[length]();

   for(int i=0;i<length;i++)
   {
       data[i]=dataDouble[i+20];
   }


    //以下对data进行处理，包括删除3/4和倒相

    if (m_rscctype == 2)//判断是否需要做删除冗余
    {
        float *dataTwo = new float[(int)length*2/3]();
        int c=0,m=length % 6;
        for(int i=1;i<length;i++)
        {

            if((i+1)%6 == 0)
            {
                    dataTwo[c*4]=data[i-5];
                    dataTwo[c*4+1]=data[i-4];
                    dataTwo[c*4+2]=data[i-2];
                    dataTwo[c*4+3]=data[i-1];
                    c++;
            }
        }

        //末位处理

        if(m == 2)
        {
            dataTwo[c*4]=data[length-2];
            dataTwo[c*4+1]=data[length-1];
            length=((int)length/6)*4+2;
        } else if(m== 4)
        {
            dataTwo[c*4]=data[length-4];
            dataTwo[c*4+1]=data[length-3];
            dataTwo[c*4+2]=data[length-1];
            length=((int)length/6)*4+3;
        }

        for(int i=0;i<length;i++)
        {
            data[i]=dataTwo[i];
            qDebug()<<"data2 "<<i<<" "<<data[i];
        }
    }

    if (m_rsccphase == 1){
        for(int i=0;i<length;i++)
        {
            if(i%2 == 0)
                data[i+1]=!data[i+1];
        }
    } else if(m_rsccphase == 2){
        int a;
        for(int i=0;i<length;i++)
        {
            if(i%2 == 1)
            {
                a = data[i-1];
                data[i-1]=data[i];
                data[i]=a;
            }
        }
    } else if(m_rsccphase == 3){
        int a;
        for(int i=0;i<length;i++)
        {
            if(i%2 == 1)
            {
                a = data[i-1];
                data[i-1]=!data[i];
                data[i]=a;
            }
        }
    }

    Waveform res("", length, data);
    delete [] data;
    delete [] dataDouble;
    delete [] regStatus;
    return res;
}

void RSCCGenerator::setG(char16_t g[], size_t  cnt)
{
    m_g = new char16_t[cnt]();
    for(size_t i = 0; i < cnt; i++)
        m_g[i] = g[i];
}

