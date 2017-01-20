#include "ldpc_parawidget.h"
#include "ui_ldpc_parawidget.h"
#include "ldpcgenerator.h"

LDPC_ParaWidget::LDPC_ParaWidget(EncodingParaWidget *parent) :
    EncodingParaWidget(parent),
    ui(new Ui::LDPC_ParaWidget)
{
    ui->setupUi(this);
}

LDPC_ParaWidget::~LDPC_ParaWidget()
{
    delete ui;
}

Waveform LDPC_ParaWidget::outputWithWaveform(const Waveform &inputWfm,int a)
{
    if (ui->cbxEncodingType->currentIndex() == 0)
        return inputWfm;
    else {
        LDPCGenerator generator;
        return generator.outputWithInputWfm(inputWfm,a);
    }
}
