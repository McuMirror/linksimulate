#include "instrcontrolwidget.h"
#include "ui_instrcontrolwidget.h"

#include <QMessageBox>
#include "instrumentcontroller.h"
#include "statusdescription.h"

#include <iostream>


InstrControlWidget::InstrControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrControlWidget)
{
    ui->setupUi(this);
}

InstrControlWidget::~InstrControlWidget()
{
    delete ui;
}

void InstrControlWidget::on_btnConnect_clicked()
{
    try {
        QString instrAddress = ui->leAddres->text();
        InstrumentController::getInstance()->openSession(instrAddress.toStdString(), InstrumentController::TCPIP);
        if(InstrumentController::getInstance()->currentConnectionStatus() == InstrumentController::Connected) {
            QMessageBox *mb = new QMessageBox(QMessageBox::Information,
                                              "Success",
                                              "Successfully connect to instrument.",
                                              QMessageBox::Ok);
            mb->show();
        }
        ui->leAddres->setEnabled(false);
    }
    catch(std::exception e) {
        QMessageBox *mb = new QMessageBox(QMessageBox::Warning,
                                          "Error",
                                          e.what(),
                                          QMessageBox::Ok);
        mb->show();
    }
}

void InstrControlWidget::on_btnDisconnect_clicked()
{
    try {
        if(InstrumentController::getInstance()->currentConnectionStatus() != InstrumentController::Disconnected)
            InstrumentController::getInstance()->closeCurrnetSession();
        if(InstrumentController::getInstance()->currentConnectionStatus() == InstrumentController::Disconnected) {
            QMessageBox *mb = new QMessageBox(QMessageBox::Information,
                                              "Success",
                                              "Successfully close the connection.",
                                              QMessageBox::Ok);
            mb->show();
        }
        ui->leAddres->setEnabled(true);
    }
    catch(std::exception e) {
        QMessageBox *mb = new QMessageBox(QMessageBox::Warning,
                                          "Error",
                                          e.what(),
                                          QMessageBox::Ok);
        mb->show();
    }
}

void InstrControlWidget::on_playButton_clicked()
{
   InstrumentController::getInstance()->playInstrument();
}

void InstrControlWidget::on_stopButton_clicked()
{
   InstrumentController::getInstance()->stopInstrument();
}
