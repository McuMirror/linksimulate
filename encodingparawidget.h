#ifndef ENCODINGPARAWIDGET_H
#define ENCODINGPARAWIDGET_H

#include <QWidget>
#include <QDebug>

#include "encoder.h"

class Waveform;
//class QLineEdit;

//class IEncodingParaWidget {
//public:
//    virtual Encoder * encoder() = 0;
//};

class EncodingParaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EncodingParaWidget(QWidget *parent = 0);
    virtual ~EncodingParaWidget();

    virtual Waveform outputWithWaveform(const Waveform &inputWfm, int a);

//    virtual Encoder *           encoder() { return m_encoder = NULL; }

protected:
//    virtual void                inputError(QLineEdit *lineEdit);

private:
//    Encoder *                   m_encoder;

signals:

public slots:
};

#endif // ENCODINGPARAWIDGET_H
