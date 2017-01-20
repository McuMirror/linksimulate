#include "simulationoutputwidget.h"
#include "ui_simulationoutputwidget.h"
#include "simanimationwidget.h"
#include "linkcalculation.h"
#include "calculationresultsdialog.h"
#include <QDebug>

SimulationOutputWidget::SimulationOutputWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationOutputWidget)
{
    ui->setupUi(this);
    m_startLongitude = 0.f;
    m_endLongitude = 120.f;

    ui->animationWidget->setup();

    connect(ui->animationWidget, SIGNAL(animationProgressUpdated(float)), this, SLOT(updateAnimationProgress(float)));

    m_linkCalculator = new LinkCalculation;
}

SimulationOutputWidget::~SimulationOutputWidget()
{
    delete ui;
    delete m_linkCalculator;
}

void SimulationOutputWidget::startSimulation()
{
    ui->animationWidget->startAnimation();
}

void SimulationOutputWidget::updateAnimationProgress(float p)
{
    if (1) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(++rowCount);
        int row = rowCount - 1;
        float longitude = m_startLongitude + p * (m_endLongitude - m_startLongitude);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(longitude)));
        ui->tableWidget->item(row, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(m_linkCalculator->satelliteAltitude())));
        ui->tableWidget->item(row, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(m_linkCalculator->earthStationLongitude())));
        ui->tableWidget->item(row, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(m_linkCalculator->earthStationLatitude())));
        ui->tableWidget->item(row, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

float SimulationOutputWidget::endLongitude() const
{
    return m_endLongitude;
}

void SimulationOutputWidget::setEndLongitude(float endLongitude)
{
    m_endLongitude = endLongitude;
}

float SimulationOutputWidget::startLongitude() const
{
    return m_startLongitude;
}

void SimulationOutputWidget::setStartLongitude(float startLongitude)
{
    m_startLongitude = startLongitude;
}

void SimulationOutputWidget::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    m_linkCalculator->setLongitudeUnderSatellite(ui->tableWidget->item(row, 0)->text().toFloat());

    qDebug() << "************************************************************************";
    qDebug() << "information rate" << m_linkCalculator->informationRate();
    qDebug() << "symbol rate" << m_linkCalculator->symbolRate();
    qDebug() << "bandwidth" << m_linkCalculator->systemBandwidth();
    qDebug() << "antenna gain" << m_linkCalculator->antennaGain();
    qDebug() << "system equivalent noise" << m_linkCalculator->systemEquivalentNoise();
    qDebug() << "receiver antenna G/T" << m_linkCalculator->receiverAntennaGOverT();
    qDebug() << "distance" << m_linkCalculator->distanceBetweenStationAndSatellite();
    qDebug() << "uplink transmission loss" << m_linkCalculator->uplinkTransmissionLoss();
    qDebug() << "downlink transmission loss" << m_linkCalculator->downlinkTransmissionLoss();
    qDebug() << "uplink total loss" << m_linkCalculator->uplinkTotalLoss();
    qDebug() << "downlink total loss" << m_linkCalculator->downlinkTotalLoss();
    qDebug() << "IPBOes" << m_linkCalculator->IPBOes();
    qDebug() << "uplink C/T without interference" << m_linkCalculator->uplinkCOverTWithoutInterference();
    qDebug() << "uplink C/N without interference" << m_linkCalculator->uplinkCOverNWithoutInterference();
    qDebug() << "uplink C/T with interference" << m_linkCalculator->uplinkCOverTWithInterference();
    qDebug() << "uplink C/N with interference" << m_linkCalculator->uplinkCOverNWithInterference();
    qDebug() << "OPBOes" << m_linkCalculator->OPBOes();
    qDebug() << "downlink C/T without interference" << m_linkCalculator->downlinkCOverTWithoutInterference();
    qDebug() << "downlink C/N without interference" << m_linkCalculator->downlinkCOverNWithoutInterference();
    qDebug() << "downlink C/T with interference" << m_linkCalculator->downlinkCOverTWithInterference();
    qDebug() << "downlink C/N with interference" << m_linkCalculator->downlinkCOverNWithInterference();
    qDebug() << "link total C/T" << m_linkCalculator->linkTotalCOverT();
    qDebug() << "link total C/N" << m_linkCalculator->linkTotalCOverN();
    qDebug() << "Eb/N0" << m_linkCalculator->EbOverN0();
    qDebug() << "Eb/N0 margin" << m_linkCalculator->EbOverN0Margin();
    qDebug() << "antenna diameter" << m_linkCalculator->antennaDiameter();
    qDebug() << "************************************************************************\n";

    CalculationResultsDialog *resultsDialog = new CalculationResultsDialog(this);
    resultsDialog->setupCalculationResults(m_linkCalculator);
    resultsDialog->show();
}

LinkCalculation *SimulationOutputWidget::linkCalculator() const
{
    return m_linkCalculator;
}

void SimulationOutputWidget::setLinkCalculator(LinkCalculation *linkCalculator)
{
    m_linkCalculator = linkCalculator;
}

void SimulationOutputWidget::on_btnRunSimulation_clicked()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->animationWidget->startAnimation();
}
