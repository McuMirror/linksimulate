#ifndef WFMIMPORTDIALOG_H
#define WFMIMPORTDIALOG_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class WfmImportDialog;
}

class WfmImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WfmImportDialog(QWidget *parent = 0);
    ~WfmImportDialog();

private slots:
    void switchRadioButton();
    void on_btnConfirm_clicked();
    void on_btnCancel_clicked();

    void on_btnScan_clicked();

private:
    Ui::WfmImportDialog *           ui;
    QButtonGroup *                  m_radioButtonGroup;
    QString                         m_pathMATFile;
};

#endif // WFMIMPORTDIALOG_H
