#include "frameconstructor.h"
#include "waveform.h"
#include <QDebug>

FrameConstructor::FrameConstructor(QObject *parent) : QObject(parent)
{

}

Waveform FrameConstructor::buildDataFrame(const Waveform &wfm)
{
    if (m_frameLength == 0)
        return wfm;

    if (m_syncWord.length() != m_headerLength * 2)
        throw std::exception("Error - Synchronizing word length doesn't match header length, please check your input.");

    size_t frameDataBit = (m_frameLength - m_headerLength) * 8; //帧中有效数据位数             // bit(point in digital signal)

   // size_t frameNum = wfm.length() % frameDataBit == 0 ? wfm.length() / frameDataBit
     //                                                    : wfm.length() / frameDataBit + 1;//帧的个数
    size_t frameNum =1;
    size_t WL=wfm.length();
    while(!((frameDataBit*frameNum) % WL ==0))
    {
        frameNum++;
    }
   qDebug()<<"frameNum"<<frameNum;

    size_t headerBit = m_headerLength * 8;//帧头位数
    size_t frameBit = m_frameLength * 8;//单帧位数
    size_t length = frameNum * frameBit;//装总长帧


    QString binaryHeaderStr;
    for (int i = 0; i < m_syncWord.length(); i++) {
        bool ok;
        int value = QString(m_syncWord[i]).toInt(&ok, 16);
        QString binaryString = QString::number(value, 2);
        if (value < 2)
            binaryString.insert(0, "000");
        else if (value < 4)
            binaryString.insert(0, "00");
        else if (value < 8)
            binaryString.insert(0, "0");
        binaryHeaderStr.append(binaryString);
    }
//    float *headerData = new float[binaryHeaderStr.length()]();
    std::vector<float> headerData(binaryHeaderStr.length(), 0);
    for (int i = 0; i < binaryHeaderStr.length(); i++)
//        headerData[i] = binaryHeaderStr[i].toAscii() - '0';
        headerData[i] = binaryHeaderStr[i].toLatin1() - '0';

//    float *data = new float[length]();


//改下边
  std::vector<float> data(length, 0);
  int numData =0;
    for (int i = 0; i < frameNum; i++)//遍历每一个帧
        {
            for (int j = 0; j < headerBit; j++)//每一个帧加帧头
                data[i * frameBit + j] = headerData[j];

            for (int k = 0; k < frameDataBit; k++)
            {
                if(numData == WL)
                   numData = 0;
                data[i * frameBit + headerBit + k] = wfm.data()[numData];
                   numData++;
            }
    }
    /*for (int i = 0; i < frameNum; i++) {
        for (int j = 0; j < headerBit; j++)
            data[i * frameBit + j] = headerData[j];
        for (int k = 0; k < frameDataBit; k++)
            if (frameDataBit * i + k < wfm.length())
                data[i * frameBit + headerBit + k] = wfm.data()[frameDataBit * i + k];
            else {
                int index = k % (wfm.length() - i * frameDataBit) + i * frameDataBit;
                data[i * frameBit + headerBit + k] = wfm.data()[index];
            }
    }*/

    Waveform outWfm("",data);

    return outWfm;
}

size_t FrameConstructor::frameLength() const
{
    return m_frameLength;
}

void FrameConstructor::setFrameLength(const size_t &frameLength)
{
    m_frameLength = frameLength;
}

size_t FrameConstructor::headerLength() const
{
    return m_headerLength;
}

void FrameConstructor::setHeaderLength(const size_t &headerLength)
{
    m_headerLength = headerLength;
}

QString FrameConstructor::syncWord() const
{
    return m_syncWord;
}

void FrameConstructor::setSyncWord(const QString &syncWord)
{
    m_syncWord = syncWord;
}
