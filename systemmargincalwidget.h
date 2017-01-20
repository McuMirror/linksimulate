#ifndef SYSTEMMARGINCALWIDGET_H
#define SYSTEMMARGINCALWIDGET_H

#include <QWidget>

namespace Ui {
class SystemMarginCalWidget;
}

class SystemMarginCalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemMarginCalWidget(QWidget *parent = 0);
    ~SystemMarginCalWidget();

private slots:
    void on_btnRunSimulation_clicked();

    void on_cbxRateChoosed_currentIndexChanged(int index);

    void on_ckbUplinkRollback_clicked();

    void on_ckbUplinkInterference_clicked();

    void on_ckbDownlinkRollback_clicked();

    void on_ckbDownlinkInterference_clicked();

    void on_btnImportSettings_clicked();

    void on_btnExportSettings_clicked();

private:
    Ui::SystemMarginCalWidget *ui;
};

#endif // SYSTEMMARGINCALWIDGET_H
