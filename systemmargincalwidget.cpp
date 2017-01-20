#include "systemmargincalwidget.h"
#include "ui_systemmargincalwidget.h"
#include "simulationoutputwidget.h"
#include "linkcalculation.h"
#include <QFileDialog>

#include <QDebug>

// Bad bad code

static const QString UnusedText = "NaN";

SystemMarginCalWidget::SystemMarginCalWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemMarginCalWidget)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    ui->ckbUplinkRollback->setChecked(true);
    ui->ckbUplinkInterference->setChecked(true);
    ui->ckbDownlinkRollback->setChecked(true);
    ui->ckbDownlinkInterference->setChecked(true);
}

SystemMarginCalWidget::~SystemMarginCalWidget()
{
    delete ui;
}

void SystemMarginCalWidget::on_btnRunSimulation_clicked()
{
    SimulationOutputWidget *sow = new SimulationOutputWidget(this);
    LinkCalculation *linkCalculator = sow->linkCalculator();

    bool isOK;
    // Basic parameters
    switch (ui->cbxRateChoosed->currentIndex()) {
    case 0:
        linkCalculator->setInformationRate(ui->leRate->text().toFloat(&isOK));
        break;
    case 1:
        linkCalculator->setSymbolRate(ui->leRate->text().toFloat(&isOK));
        break;
    default:
        break;
    }
    float FEC = ui->leFECNumerator->text().toFloat(&isOK) / ui->leFECDenominator->text().toFloat(&isOK);
    float RS = ui->leRSNumerator->text().toFloat(&isOK) / ui->leRSDenominator->text().toFloat(&isOK);
    linkCalculator->setFEC(FEC);
    linkCalculator->setFECString(QString("%1/%2").arg(ui->leFECNumerator->text().toFloat(&isOK))
                                 .arg(ui->leFECDenominator->text().toFloat(&isOK)));
    linkCalculator->setRS(RS);
    linkCalculator->setRSString(QString("%1/%2").arg(ui->leRSNumerator->text().toFloat(&isOK))
                                .arg(ui->leRSDenominator->text().toFloat(&isOK)));
    linkCalculator->setModulationFactor(ui->leM->text().toInt(&isOK));
    linkCalculator->setRolloffFactor(ui->leAlpha->text().toFloat(&isOK));
    linkCalculator->setProtectionBandwidth(ui->leProtectionBandwidth->text().toFloat(&isOK));
    linkCalculator->setUplinkFrequency(ui->leUplinkFrequency->text().toFloat(&isOK));
    linkCalculator->setDownlinkFrequency(ui->leDownlinkFrequency->text().toFloat(&isOK));
    linkCalculator->setEbN0GateValue(ui->leEbN0GateValue->text().toFloat(&isOK));
    linkCalculator->setAntennaDiameter(ui->leAntennaDiameter->text().toFloat(&isOK));
    linkCalculator->setAntennaEfficiency(ui->leAntennaEfficiency->text().toFloat(&isOK));
    linkCalculator->setEquivalentNoise(ui->leEquivalentNoise->text().toFloat(&isOK));
    linkCalculator->setNoiseTemperature(ui->leNoiseTemperature->text().toFloat(&isOK));
    linkCalculator->setEnvironmentTemperature(ui->leEnvironmentTemperature->text().toFloat(&isOK));
    linkCalculator->setFeederLoss(ui->leFeederLoss->text().toFloat(&isOK));

    // Space loss
    linkCalculator->setEarthRadius(ui->leEarthRadius->text().toFloat(&isOK));
    linkCalculator->setSatelliteAltitude(ui->leSatelliteAltitude->text().toFloat(&isOK));
    linkCalculator->setEarthStationLongitude(ui->leEarthStationLongitude->text().toFloat(&isOK));
    linkCalculator->setEarthStationLatitude(ui->leEarthStationLatitude->text().toFloat(&isOK));
    linkCalculator->setUplinkRainLoss(ui->leUplinkRainLoss->text().toFloat(&isOK));
    linkCalculator->setDownlinkRainLoss(ui->leDownlinkRainLoss->text().toFloat(&isOK));

    // Uplink C/T
    linkCalculator->setIPBO(ui->leIPBO->text().toFloat(&isOK));
    linkCalculator->setSatelliteGT(ui->leSatelliteGT->text().toFloat(&isOK));
    linkCalculator->setSFD(0 - ui->leSFD->text().toFloat(&isOK));                   // SFD less than 0
    if (ui->ckbUplinkRollback->isChecked()) {
        linkCalculator->setUplinkOccupiedBandwidth(ui->leUplinkOccupiedBandwidth->text().toFloat(&isOK));
        linkCalculator->setUplinkTransponderBandwidth(ui->leUplinkTransponderBandwidth->text().toFloat(&isOK));
        linkCalculator->setUplinkRollbackCompensation(ui->leUplinkRollbackCompensation->text().toFloat(&isOK));
    }
    if (ui->ckbUplinkInterference->isChecked()) {
        linkCalculator->setUplinkIntermodulationInterference(ui->leUplinkIntermodulationInterference->text().toFloat(&isOK));
        linkCalculator->setUplinkcrosspolarizationInterference(ui->leUplinkCrosspolarizationInterference->text().toFloat(&isOK));
        linkCalculator->setUplinkSatelliteInterference(ui->leUplinkSatelliteInterference->text().toFloat(&isOK));
    }

    // Downlink C/T
    linkCalculator->setOPBO(ui->leOPBO->text().toFloat(&isOK));
    linkCalculator->setSatelliteEIRP(ui->leSatelliteEIRP->text().toFloat(&isOK));
    if (ui->ckbDownlinkRollback->isChecked()) {
        linkCalculator->setDownlinkOccupiedBandwidth(ui->leDownlinkOccupiedBandwidth->text().toFloat(&isOK));
        linkCalculator->setDownlinkTransponderBandwidth(ui->leDownlinkTransponderBandwidth->text().toFloat(&isOK));
        linkCalculator->setDownlinkRollbackCompensation(ui->leDownlinkRollbackCompensation->text().toFloat(&isOK));
    }
    if (ui->ckbDownlinkInterference->isChecked()) {
        linkCalculator->setDownlinkIntermodulationInterference(ui->leDownlinkIntermodulationInterference->text().toFloat(&isOK));
        linkCalculator->setDownlinkcrosspolarizationInterference(ui->leDownlinkCrosspolarizationInterference->text().toFloat(&isOK));
        linkCalculator->setDownlinkSatelliteInterference(ui->leDownlinkSatelliteInterference->text().toFloat(&isOK));
    }

    sow->setStartLongitude(ui->leStartLongitude->text().toFloat());
    sow->setEndLongitude(ui->leEndLongitude->text().toFloat());
    sow->show();
}

void SystemMarginCalWidget::on_cbxRateChoosed_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->labRateUnit->setText("Mbps");
        break;
    case 1:
        ui->labRateUnit->setText("MBaud");
        break;
    default:
        break;
    }
}

void SystemMarginCalWidget::on_ckbUplinkRollback_clicked()
{
    ui->leUplinkOccupiedBandwidth->setEnabled(ui->ckbUplinkRollback->isChecked());
    ui->leUplinkTransponderBandwidth->setEnabled(ui->ckbUplinkRollback->isChecked());
    ui->leUplinkRollbackCompensation->setEnabled(ui->ckbUplinkRollback->isChecked());
}

void SystemMarginCalWidget::on_ckbUplinkInterference_clicked()
{
    ui->leUplinkIntermodulationInterference->setEnabled(ui->ckbUplinkInterference->isChecked());
    ui->leUplinkCrosspolarizationInterference->setEnabled(ui->ckbUplinkInterference->isChecked());
    ui->leUplinkSatelliteInterference->setEnabled(ui->ckbUplinkInterference->isChecked());
}

void SystemMarginCalWidget::on_ckbDownlinkRollback_clicked()
{
    ui->leDownlinkOccupiedBandwidth->setEnabled(ui->ckbDownlinkRollback->isChecked());
    ui->leDownlinkTransponderBandwidth->setEnabled(ui->ckbDownlinkRollback->isChecked());
    ui->leDownlinkRollbackCompensation->setEnabled(ui->ckbDownlinkRollback->isChecked());
}

void SystemMarginCalWidget::on_ckbDownlinkInterference_clicked()
{
    ui->leDownlinkIntermodulationInterference->setEnabled(ui->ckbDownlinkInterference->isChecked());
    ui->leDownlinkCrosspolarizationInterference->setEnabled(ui->ckbDownlinkInterference->isChecked());
    ui->leDownlinkSatelliteInterference->setEnabled(ui->ckbDownlinkInterference->isChecked());
}

void SystemMarginCalWidget::on_btnImportSettings_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString::fromUtf8("导入"),
                                                "",
                                                "Data Files(*.dat);;All Files(*.*)");

    if (!path.isNull()) {
        for (int i = 0; i < path.length(); i++) {
            if (path[i] == '/')
                path.replace(i, 1, "\\");
        }
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QString temp;
    QDataStream in(&file);
    //
    in >> temp;
    ui->cbxRateChoosed->setCurrentIndex(temp.toInt());
    in >> temp;
    ui->leRate->setText(temp);
    in >> temp;
    ui->leFECNumerator->setText(temp);
    in >> temp;
    ui->leFECDenominator->setText(temp);
    in >> temp;
    ui->leRSNumerator->setText(temp);
    in >> temp;
    ui->leRSDenominator->setText(temp);
    in >> temp;
    ui->leM->setText(temp);
    in >> temp;
    ui->leAlpha->setText(temp);
    in >> temp;
    ui->leProtectionBandwidth->setText(temp);
    in >> temp;
    ui->leUplinkFrequency->setText(temp);
    in >> temp;
    ui->leDownlinkFrequency->setText(temp);
    in >> temp;
    ui->leEbN0GateValue->setText(temp);
    in >> temp;
    ui->leAntennaDiameter->setText(temp);
    in >> temp;
    ui->leAntennaEfficiency->setText(temp);
    in >> temp;
    ui->leEquivalentNoise->setText(temp);
    in >> temp;
    ui->leNoiseTemperature->setText(temp);
    in >> temp;
    ui->leEnvironmentTemperature->setText(temp);
    in >> temp;
    ui->leFeederLoss->setText(temp);

    //
    in >> temp;
    ui->leEarthRadius->setText(temp);
    in >> temp;
    ui->leSatelliteAltitude->setText(temp);
    in >> temp;
    ui->leEarthStationLongitude->setText(temp);
    in >> temp;
    ui->leEarthStationLatitude->setText(temp);
    in >> temp;
    ui->leStartLongitude->setText(temp);
    in >> temp;
    ui->leEndLongitude->setText(temp);
    in >> temp;
    ui->leUplinkRainLoss->setText(temp);
    in >> temp;
    ui->leDownlinkRainLoss->setText(temp);

    //
    in >> temp;
    ui->leIPBO->setText(temp);
    in >> temp;
    ui->leSatelliteGT->setText(temp);
    in >> temp;
    ui->leSFD->setText(temp);
    in >> temp;
    if (temp.compare(UnusedText)) {
        ui->leUplinkOccupiedBandwidth->setText(temp);
        in >> temp;
        ui->leUplinkTransponderBandwidth->setText(temp);
        in >> temp;
        ui->leUplinkRollbackCompensation->setText(temp);
        ui->ckbUplinkRollback->setCheckState(Qt::Checked);
    }
    else {
        ui->leUplinkOccupiedBandwidth->setText("");
        in >> temp;
        ui->leUplinkTransponderBandwidth->setText("");
        in >> temp;
        ui->leUplinkRollbackCompensation->setText("");
        ui->ckbUplinkRollback->setCheckState(Qt::Unchecked);
    }
    in >> temp;
    if (temp.compare(UnusedText)) {
        ui->leUplinkIntermodulationInterference->setText(temp);
        in >> temp;
        ui->leUplinkCrosspolarizationInterference->setText(temp);
        in >> temp;
        ui->leUplinkSatelliteInterference->setText(temp);
        ui->ckbUplinkInterference->setCheckState(Qt::Checked);
    }
    else {
        ui->leUplinkIntermodulationInterference->setText("");
        in >> temp;
        ui->leUplinkCrosspolarizationInterference->setText("");
        in >> temp;
        ui->leUplinkSatelliteInterference->setText("");
        ui->ckbUplinkInterference->setCheckState(Qt::Unchecked);
    }

    //
    in >> temp;
    ui->leOPBO->setText(temp);
    in >> temp;
    ui->leSatelliteEIRP->setText(temp);
    in >> temp;
    if (temp.compare(UnusedText)) {
        ui->leDownlinkOccupiedBandwidth->setText(temp);
        in >> temp;
        ui->leDownlinkTransponderBandwidth->setText(temp);
        in >> temp;
        ui->leDownlinkRollbackCompensation->setText(temp);
        ui->ckbDownlinkRollback->setCheckState(Qt::Checked);
    }
    else {
        ui->leDownlinkOccupiedBandwidth->setText("");
        in >> temp;
        ui->leDownlinkTransponderBandwidth->setText("");
        in >> temp;
        ui->leDownlinkRollbackCompensation->setText("");
        ui->ckbDownlinkRollback->setCheckState(Qt::Unchecked);
    }
    in >> temp;
    if (temp.compare(UnusedText)) {
        ui->leDownlinkIntermodulationInterference->setText(temp);
        in >> temp;
        ui->leDownlinkCrosspolarizationInterference->setText(temp);
        in >> temp;
        ui->leDownlinkSatelliteInterference->setText(temp);
        ui->ckbDownlinkInterference->setCheckState(Qt::Checked);
    }
    else {
        ui->leDownlinkIntermodulationInterference->setText("");
        in >> temp;
        ui->leDownlinkCrosspolarizationInterference->setText("");
        in >> temp;
        ui->leDownlinkSatelliteInterference->setText("");
        ui->ckbDownlinkInterference->setCheckState(Qt::Unchecked);
    }

    file.close();
}

void SystemMarginCalWidget::on_btnExportSettings_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                QString::fromUtf8("保存"),
                                                "",
                                                "Data Files(*.dat);;All Files(*.*)");

    if (!path.isNull()) {
        for (int i = 0; i < path.length(); i++) {
            if (path[i] == '/')
                path.replace(i, 1, "\\");
        }
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QDataStream out(&file);
    //
    out << QString::number(ui->cbxRateChoosed->currentIndex());
    out << ui->leRate->text();
    out << ui->leFECNumerator->text() << ui->leFECDenominator->text();
    out << ui->leRSNumerator->text() << ui->leRSDenominator->text();
    out << ui->leM->text();
    out << ui->leAlpha->text();
    out << ui->leProtectionBandwidth->text();
    out << ui->leUplinkFrequency->text();
    out << ui->leDownlinkFrequency->text();
    out << ui->leEbN0GateValue->text();
    out << ui->leAntennaDiameter->text();
    out << ui->leAntennaEfficiency->text();
    out << ui->leEquivalentNoise->text();
    out << ui->leNoiseTemperature->text();
    out << ui->leEnvironmentTemperature->text();
    out << ui->leFeederLoss->text();

    //
    out << ui->leEarthRadius->text();
    out << ui->leSatelliteAltitude->text();
    out << ui->leEarthStationLongitude->text();
    out << ui->leEarthStationLatitude->text();
    out << ui->leStartLongitude->text();
    out << ui->leEndLongitude->text();
    out << ui->leUplinkRainLoss->text();
    out << ui->leDownlinkRainLoss->text();

    //
    out << ui->leIPBO->text();
    out << ui->leSatelliteGT->text();
    out << ui->leSFD->text();
    if (ui->ckbUplinkRollback->isChecked()) {
        out << ui->leUplinkOccupiedBandwidth->text();
        out << ui->leUplinkTransponderBandwidth->text();
        out << ui->leUplinkRollbackCompensation->text();
    }
    else {
        out << UnusedText;
        out << UnusedText;
        out << UnusedText;
    }
    if (ui->ckbUplinkInterference->isChecked()) {
        out << ui->leUplinkIntermodulationInterference->text();
        out << ui->leUplinkCrosspolarizationInterference->text();
        out << ui->leUplinkSatelliteInterference->text();
    }
    else {
        out << UnusedText;
        out << UnusedText;
        out << UnusedText;
    }

    //
    out << ui->leOPBO->text();
    out << ui->leSatelliteEIRP->text();
    if (ui->ckbDownlinkRollback->isChecked()) {
        out << ui->leDownlinkOccupiedBandwidth->text();
        out << ui->leDownlinkTransponderBandwidth->text();
        out << ui->leDownlinkRollbackCompensation->text();
    }
    else {
        out << UnusedText;
        out << UnusedText;
        out << UnusedText;
    }
    if (ui->ckbDownlinkInterference->isChecked()) {
        out << ui->leDownlinkIntermodulationInterference->text();
        out << ui->leDownlinkCrosspolarizationInterference->text();
        out << ui->leDownlinkSatelliteInterference->text();
    }
    else {
        out << UnusedText;
        out << UnusedText;
        out << UnusedText;
    }

    file.close();
}
