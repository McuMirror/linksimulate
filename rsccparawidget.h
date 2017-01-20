#ifndef RSCCPARAWIDGET_H
#define RSCCPARAWIDGET_H

#include <QWidget>
#include "encodingparawidget.h"
#include "encoder.h"
#include "rsccgenerator.h"

namespace Ui {
class RSCCParaWidget;
}

class RSCCParaWidget : public EncodingParaWidget
{
    Q_OBJECT

public:
    explicit RSCCParaWidget(EncodingParaWidget *parent = 0);
    ~RSCCParaWidget();

    virtual Encoder * encoder() { return m_RSCCGenerator; }

private slots:
    void on_btnConfirm_clicked();

private:
    Ui::RSCCParaWidget *            ui;
    RSCCGenerator *                 m_RSCCGenerator;

    bool getG(char16_t g[], size_t &count);            // If the original g array is not empty, it will be covered.
};

#endif // RSCCPARAWIDGET_H
