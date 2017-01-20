#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *            ui;
    QLabel *                    msgLabel;

private slots:
    void stateChangeToConnected();
    void stateChangeToDisconnected();
};

#endif // MAINWINDOW_H
