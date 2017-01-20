#include "instrumentcontroller.h"

#include <QThread>

#pragma comment(lib, "visa64.lib")

InstrumentController * InstrumentController::m_instance = NULL;

int channel=1;

InstrumentController::InstrumentController(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<Waveform>("Waveform");
    connect(this, SIGNAL(sigSendWaveform(Waveform)), this, SLOT(slotSendWaveform(Waveform)));

    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();

    // Create s session to the Default Resource Manager
    m_connectionStatus = Disconnected;
    m_status = viOpenDefaultRM(&m_resourceManager);
    if(m_status < VI_SUCCESS){
        throwException(m_resourceManager, m_status);
    }
}

InstrumentController::~InstrumentController()
{
    delete m_instance;
    delete m_thread;
}

void InstrumentController::openSession(std::string instrAddress, Mode mode)
{
    qDebug() << "open session in thread:" << QThread::currentThreadId();

    std::string preStr, tailStr;
    switch (mode) {
    case TCPIP:
        preStr = "TCPIP::";
        tailStr = "::INSTR";
        break;
    default:
        break;
    }
    instrAddress = preStr + instrAddress + tailStr;

    m_status = viOpen(m_resourceManager, (ViRsrc)instrAddress.c_str(), VI_NULL, VI_NULL, &m_session);
    if(m_status < VI_SUCCESS)
        throwException(m_session, m_status);
    else {
        m_connectionStatus = Connected;
        emit instrumentConnected();
    }
}

void InstrumentController::closeCurrnetSession()
{
    if(m_status = viClose(m_session) < VI_SUCCESS)
        throwException(m_session, m_status);
    else {
        m_connectionStatus = Disconnected;
        emit instrumentDisconnected();
    }
}

void InstrumentController::initializeSettings()
{
    ViUInt32 retCnt;
    m_status = viWrite(m_session, (ViBuf)"*CLS;*RST\n", 10, &retCnt);
    if(m_status < VI_SUCCESS)
        throwException(m_session, m_status);
}

void InstrumentController::sendWaveform(const Waveform &wfm)
{
    emit sigSendWaveform(wfm);
}

void InstrumentController::slotSendWaveform(const Waveform &wfm)
{
    qDebug() << "send waveform in thread:" << QThread::currentThreadId();
    qDebug() << "sending waveform name:" << wfm.name().c_str();

    // Do not use viPrintf()
    ViUInt32 retCnt;

    size_t bytes = wfm.length() * sizeof(float);
    char length[10];
    sprintf_s(length, 10, "%d", bytes);

    sprintf_s(m_buffer, BUFFER_SIZE, "WLIS:WAV:NEW \"%s\",%d\n"
              , wfm.name().c_str(), wfm.length());
    m_status = viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
    sprintf_s(m_buffer, BUFFER_SIZE, "WLIS:WAV:DATA \"%s\",#%d%s"
              , wfm.name().c_str(), strlen(length), length);
    m_status = viSetAttribute(m_session, VI_ATTR_SEND_END_EN, VI_FALSE);
    if(m_status >= VI_SUCCESS)
        m_status = viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
    if(m_status >= VI_SUCCESS)
        m_status = viWrite(m_session, (ViBuf)wfm.binblock(), (ViUInt32)bytes, &retCnt);
    if(m_status >= VI_SUCCESS)
        m_status = viSetAttribute(m_session, VI_ATTR_SEND_END_EN, VI_TRUE);
    if(m_status >= VI_SUCCESS)
        m_status = viWrite(m_session, (ViBuf)"\n", 1, &retCnt);

   //OFF two channels
    sprintf_s(m_buffer, BUFFER_SIZE, "OUTPUT1:STATE OFF");
    viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
    sprintf_s(m_buffer, BUFFER_SIZE, "*OPC?");
    viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);

    sprintf_s(m_buffer, BUFFER_SIZE, "OUTPUT2:STATE OFF");
    viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
    sprintf_s(m_buffer, BUFFER_SIZE, "*OPC?");
    viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);

    //assigns a waveform to the specified channel
   // qDebug() <<"channel"<<channel;
    if(channel == 1)
    {
        sprintf_s(m_buffer, BUFFER_SIZE, "SOURCE1:CASSET:WAVEFORM \"%s\""
                  , wfm.name().c_str());
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);

        //ON channel 1
        sprintf_s(m_buffer, BUFFER_SIZE, "OUTPUT1:STATE ON");
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
        sprintf_s(m_buffer, BUFFER_SIZE, "*OPC?");
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
    }
    if(channel == 2)
    {
        sprintf_s(m_buffer, BUFFER_SIZE, "SOURCE2:CASSET:WAVEFORM \"%s\""
                  , wfm.name().c_str());
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);

        //ON channel 2
        sprintf_s(m_buffer, BUFFER_SIZE, "OUTPUT2:STATE ON");
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
        sprintf_s(m_buffer, BUFFER_SIZE, "*OPC?");
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
    }
    if(channel == 3)
    {
        sprintf_s(m_buffer, BUFFER_SIZE, "SOURCE1:CASSET:WAVEFORM \"%s\""
                  , wfm.name().c_str());
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
        sprintf_s(m_buffer, BUFFER_SIZE, "SOURCE2:CASSET:WAVEFORM \"%s\""
                  , wfm.name().c_str());
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);

        // opean channel 1
        sprintf_s(m_buffer, BUFFER_SIZE, "OUTPUT1:STATE ON");
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
        sprintf_s(m_buffer, BUFFER_SIZE, "*OPC?");
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);

        // opean channel 2
        sprintf_s(m_buffer, BUFFER_SIZE, "OUTPUT2:STATE ON");
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
        sprintf_s(m_buffer, BUFFER_SIZE, "*OPC?");
        viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
    }
    qDebug() << "complete sending...";
}
void InstrumentController::chooseChannel(int channelname)
 {
     channel = channelname;//有效的
 }

void InstrumentController::playInstrument()
{
   ViUInt32 retCnt;
   sprintf_s(m_buffer, BUFFER_SIZE, "AWGCONTROL:RUN:IMMEDIATE");
   viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
   sprintf_s(m_buffer, BUFFER_SIZE, "*OPC?");
   viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
   qDebug() << "playInstrument...";
}

void InstrumentController::stopInstrument()
{
   ViUInt32 retCnt;
   sprintf_s(m_buffer, BUFFER_SIZE, "AWGCONTROL:STOP:IMMEDIATE");
   viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
   sprintf_s(m_buffer, BUFFER_SIZE, "*OPC?");
   viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
   qDebug() << "stopInstrument...";
}
void InstrumentController::setSampleRate(float S_Rate)
{
   ViUInt32 retCnt;
   sprintf_s(m_buffer, BUFFER_SIZE, "SOURCE:FREQUENCY %fE6", S_Rate);
   viWrite(m_session, (ViBuf)m_buffer, (ViUInt32)strlen(m_buffer), &retCnt);
}
