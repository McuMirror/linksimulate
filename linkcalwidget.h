#ifndef LINKCALWIDGET_H
#define LINKCALWIDGET_H

#include <QWidget>

class LinkCalculation;

namespace Ui {
class LinkCalWidget;
}

class LinkCalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LinkCalWidget(QWidget *parent = 0);
    ~LinkCalWidget();

private slots:
    void on_btnAntGainCal_clicked();

    void on_btnFreeSpaceLossCal_clicked();

    void on_btnInfoRateCal_clicked();

    void on_btnTransRateCal_clicked();

    void on_btnSymRateCal_clicked();

    void on_btnSysBwCal_clicked();

    void on_btnAngleCal_clicked();

    void on_btnEquivalentNoiseCal_clicked();

    void on_btnInputFallbackIPBOesCal_clicked();

    void on_btnUploadEIRPesCal_clicked();

private:
    Ui::LinkCalWidget *                     ui;
    LinkCalculation *                       m_linkCalculator;
};

#endif // LINKCALWIDGET_H
