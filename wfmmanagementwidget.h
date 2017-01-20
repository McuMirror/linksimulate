#ifndef WFMMANAGEMENTWIDGET_H
#define WFMMANAGEMENTWIDGET_H

#include <QWidget>

namespace Ui {
class WfmManagementWidget;
}

class WfmManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WfmManagementWidget(QWidget *parent = 0);
    ~WfmManagementWidget();

private slots:
    void on_btnImport_clicked();
    void updateTableWidget();

    void on_btnExport_clicked();

private:
    Ui::WfmManagementWidget *ui;
};

#endif // WFMMANAGEMENTWIDGET_H
