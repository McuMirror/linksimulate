#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <instrumentcontroller.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    msgLabel = new QLabel;
    msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(msgLabel);

    this->setWindowState(Qt::WindowMaximized);

    connect(InstrumentController::getInstance(), SIGNAL(instrumentConnected()), this, SLOT(stateChangeToConnected()));
    connect(InstrumentController::getInstance(), SIGNAL(instrumentDisconnected()), this, SLOT(stateChangeToConnected()));

    this->setStyleSheet("MainWindow {background-color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, \
                        stop:0 rgba(9, 51, 87, 255), \
                        stop:1 rgba(9, 66, 119, 255));}");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete msgLabel;
}

void MainWindow::stateChangeToConnected()
{
    msgLabel->setText(QString::fromUtf8("Connected"));
}

void MainWindow::stateChangeToDisconnected()
{
    msgLabel->setText(QString::fromUtf8("Disconnected"));
}
