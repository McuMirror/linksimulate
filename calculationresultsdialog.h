#ifndef CALCULATIONRESULTSDIALOG_H
#define CALCULATIONRESULTSDIALOG_H

#include <QDialog>

namespace Ui {
class CalculationResultsDialog;
}

class LinkCalculation;

class CalculationResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalculationResultsDialog(QWidget *parent = 0);
    ~CalculationResultsDialog();
    void setupCalculationResults(LinkCalculation *linkCalculator);

private slots:
    void on_btnConfirm_clicked();

    void on_btnExport_clicked();

private:
    Ui::CalculationResultsDialog *ui;
};

#endif // CALCULATIONRESULTSDIALOG_H
