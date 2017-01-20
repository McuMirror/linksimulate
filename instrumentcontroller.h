#ifndef INSTRUMENTCONTROLLER_H
#define INSTRUMENTCONTROLLER_H


#include <QObject>
#include "waveform.h"
#include "TekVISA.h"
#include "statusdescription.h"
#include <QDebug>

#define LINEFEED_CHAR       0x0a
#define MARKER1_BIT         0x40
#define MARKER2_BIT         0x80
#define BUFFER_SIZE         512


class InstrumentController : public QObject
{
    Q_OBJECT;

public:
    typedef enum : int {
        TCPIP = 0
    } Mode;

    typedef enum : int {
        Connecting = 0,
        Connected,
        Disconnected
    } ConnectionStatus;

    static InstrumentController *       getInstance() {
                                            if(m_instance == NULL)
                                                m_instance = new InstrumentController;
                                            return m_instance;
                                        }
    ~InstrumentController();
    ConnectionStatus                    currentConnectionStatus() const { return m_connectionStatus; }
    void                                openSession(std::string instrAddress, Mode mode);
    void                                closeCurrnetSession();
    void                                initializeSettings();
    void                                sendWaveform(const Waveform &wfm);
    static void                         chooseChannel(int channelname);
    void                                playInstrument();
    void                                stopInstrument();
    void                         setSampleRate(float S_Rate);
signals:
    void                                sigSendWaveform(const Waveform &wfm);
    void                                instrumentConnected();
    void                                instrumentDisconnected();

private slots:
    void                                slotSendWaveform(const Waveform &wfm);

private:
    InstrumentController(QObject *parent = 0);
    InstrumentController(const InstrumentController &) {}
    InstrumentController & operator=(const InstrumentController &) {}
    void throwException(ViSession session, ViStatus status)
    {
        char buffer[BUFFER_SIZE];
        StatusDescription des;
        des.statusDesc(session, status, buffer);
        throw std::exception(buffer);
    }

    static InstrumentController *       m_instance;
    QThread *                           m_thread;
    ViSession                           m_resourceManager;
    ViSession                           m_session;
    ViStatus                            m_status;
    ViChar                              m_buffer[BUFFER_SIZE];
    ConnectionStatus                    m_connectionStatus;
};

#endif // INSTRUMENTCONTROLLER_H
