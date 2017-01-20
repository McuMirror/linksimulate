#include "rsccparawidget.h"
#include "ui_rsccparawidget.h"
#include <exception>
#include <QMessageBox>

RSCCParaWidget::RSCCParaWidget(EncodingParaWidget *parent)
    : EncodingParaWidget(parent)
    , ui(new Ui::RSCCParaWidget)
    , m_RSCCGenerator(new RSCCGenerator)
{
    ui->setupUi(this);
}

RSCCParaWidget::~RSCCParaWidget()
{
    delete ui;
    delete m_RSCCGenerator;
}

void RSCCParaWidget::on_btnConfirm_clicked()// 这个CPP是没用的，忽略，忽略
{
    bool OK = false;
    size_t k, n, m;
    char16_t *g = NULL;

    try {
//        k = ui->leKValue->text().toUInt(&OK, 10);
//        if(!OK) {
//            inputError(ui->leKValue);
//            throw std::exception();
//        }

//        n =  ui->leNValue->text().toUInt(&OK, 10);
//        if(!OK) {
//            inputError(ui->leNValue);
//            throw std::exception();
//        }

//        m = ui->leMValue->text().toUInt(&OK, 10);
//        if(!OK) {
//            inputError(ui->leMValue);
//            throw std::exception();
//        }

//        size_t count = 0;
//        if(!getG(g, count) || count != n * k) {
//            inputError(ui->leGValue);
//            throw std::exception();
//        }
    }
    catch(std::exception e) {
        QMessageBox *mb = new QMessageBox(QMessageBox::Warning,
                                          "Error",
                                          "Error - There are errors in the parameters you set, please check your input.",
                                          QMessageBox::Ok);
        mb->show();
    }
}

bool RSCCParaWidget::getG(char16_t g[], size_t &count)
{
    QStringList list = ui->leGValue->text().split(",");

    count = list.length();

    char16_t *newG = new char16_t[list.length()]();
    bool OK;
    for(int i = 0; i < count; i++) {
        QString strG = list[i];
        OK = false;
        char16_t aG = strG.toUInt(&OK, 10);
        if(OK) {
            newG[i] = aG;
        } else {
            break;
        }
    }

    if(OK) {
        delete [] g;
        g = newG;
    }

    delete [] newG;

    return OK;
}
