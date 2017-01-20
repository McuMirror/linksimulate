#ifndef LDPC_PARAWIDGET_H
#define LDPC_PARAWIDGET_H

#include <QWidget>
#include "encodingparawidget.h"
#include "encoder.h"

namespace Ui {
class LDPC_ParaWidget;
}

class LDPC_ParaWidget : public EncodingParaWidget
{
    Q_OBJECT

public:
    explicit LDPC_ParaWidget(EncodingParaWidget *parent = 0);
    virtual ~LDPC_ParaWidget();

    virtual Waveform outputWithWaveform(const Waveform &inputWfm, int a);

private:
    Ui::LDPC_ParaWidget *ui;
};

#endif // LDPC_PARAWIDGET_H
