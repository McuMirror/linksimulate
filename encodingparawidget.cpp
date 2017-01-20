#include "encodingparawidget.h"
#include <QLineEdit>

EncodingParaWidget::EncodingParaWidget(QWidget *parent) : QWidget(parent)
{

}

EncodingParaWidget::~EncodingParaWidget()
{

}

Waveform EncodingParaWidget::outputWithWaveform(const Waveform &inputWfm,int a)
{
    return inputWfm;
}

//void EncodingParaWidget::inputError(QLineEdit *lineEdit)
//{
//    if(lineEdit->text().length() == 0)
//        lineEdit->setText(" ");
//    lineEdit->selectAll();
//    lineEdit->setFocus();
//}

