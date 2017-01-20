#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "instrcontrolwidget.h"
#include "wfmmanagementwidget.h"
#include "mainoperationwidget.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    InstrControlWidget *instrControlWidget() const;

private:
    Ui::MainWidget *ui;
//    InstrControlWidget *        m_instrControlWidget;
//    WfmManagementWidget *       m_wfmManagementWidget;
//    MainOperationWidget *       m_mainOperationWidget;
};

#endif // MAINWIDGET_H
