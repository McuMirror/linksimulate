#ifndef SIMULATIONOUTPUTWIDGET_H
#define SIMULATIONOUTPUTWIDGET_H

#include <QDialog>

class LinkCalculation;

namespace Ui {
class SimulationOutputWidget;
}

class SimulationOutputWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationOutputWidget(QWidget *parent = 0);
    ~SimulationOutputWidget();
    void startSimulation();

    float startLongitude() const;
    void setStartLongitude(float startLongitude);

    float endLongitude() const;
    void setEndLongitude(float endLongitude);

    LinkCalculation *linkCalculator() const;
    void setLinkCalculator(LinkCalculation *linkCalculator);

private:
    Ui::SimulationOutputWidget *ui;

private slots:
    void updateAnimationProgress(float p);

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_btnRunSimulation_clicked();

private:
    float                       m_startLongitude;
    float                       m_endLongitude;
    LinkCalculation *           m_linkCalculator;
};

#endif // SIMULATIONOUTPUTWIDGET_H
