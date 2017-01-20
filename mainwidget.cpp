#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QHBoxLayout>
#include <QSplitter>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

//    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
//    QSplitter *leftSplitter = new QSplitter(Qt::Vertical, mainSplitter);
//    m_instrControlWidget = new InstrControlWidget(leftSplitter);
//    m_wfmManagementWidget = new WfmManagementWidget(leftSplitter);
//    m_mainOperationWidget = new MainOperationWidget(mainSplitter);
//    leftSplitter->setStretchFactor(1, 0);
////    mainSplitter->setStretchFactor(1, 1);

//    QHBoxLayout *mainLayout = new QHBoxLayout(this);
//    mainLayout->addWidget(mainSplitter);
//    this->setLayout(mainLayout);

//    mainSplitter->show();
}

MainWidget::~MainWidget()
{
    delete ui;
//    delete m_instrControlWidget;
//    delete m_wfmManagementWidget;
//    delete m_mainOperationWidget;
}

InstrControlWidget *MainWidget::instrControlWidget() const
{
//    return m_instrControlWidget;
    return ui->instrControlWidget;
}
