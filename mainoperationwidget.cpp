#include "mainoperationwidget.h"
#include "ui_mainoperationwidget.h"

MainOperationWidget::MainOperationWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainOperationWidget)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
}

MainOperationWidget::~MainOperationWidget()
{
    delete ui;
}
