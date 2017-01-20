#ifndef FRAMECONSTRUCTOR_H
#define FRAMECONSTRUCTOR_H

#include <QObject>

class Waveform;

class FrameConstructor : public QObject
{
    Q_OBJECT
public:
    explicit FrameConstructor(QObject *parent = 0);

    Waveform buildDataFrame(const Waveform &wfm);

    size_t frameLength() const;
    void setFrameLength(const size_t &frameLength);

    size_t headerLength() const;
    void setHeaderLength(const size_t &headerLength);

    QString syncWord() const;
    void setSyncWord(const QString &syncWord);

signals:

public slots:

private:
    size_t              m_frameLength;                  // byte
    size_t              m_headerLength;                 // byte
    QString             m_syncWord;                     // Hex
};

#endif // FRAMECONSTRUCTOR_H
