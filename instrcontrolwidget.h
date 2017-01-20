#ifndef INSTRCONTROLWIDGET_H
#define INSTRCONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class InstrControlWidget;
}

class InstrControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InstrControlWidget(QWidget *parent = 0);
    ~InstrControlWidget();

private slots:
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();

    void on_playButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::InstrControlWidget *ui;
};

#endif // INSTRCONTROLWIDGET_H
