#ifndef MAINOPERATIONWIDGET_H
#define MAINOPERATIONWIDGET_H

#include <QWidget>

namespace Ui {
class MainOperationWidget;
}

class MainOperationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainOperationWidget(QWidget *parent = 0);
    ~MainOperationWidget();

private slots:

private:
    Ui::MainOperationWidget *   ui;
};

#endif // MAINOPERATIONWIDGET_H
