#ifndef RSCC_RS_PARAWIDGET_H
#define RSCC_RS_PARAWIDGET_H

#include <QWidget>
#include "encodingparawidget.h"
#include "encoder.h"

namespace Ui {
class RSCC_RS_ParaWidget;
}

class RSCC_RS_ParaWidget : public EncodingParaWidget
{
    Q_OBJECT

public:
    explicit RSCC_RS_ParaWidget(EncodingParaWidget *parent = 0);
    virtual ~RSCC_RS_ParaWidget();

    virtual Waveform outputWithWaveform(const Waveform &inputWfm, int a);

private:
    Ui::RSCC_RS_ParaWidget *ui;

    bool getG(std::vector<char16_t> &g);
};

#endif // RSCC_RS_PARAWIDGET_H
