#include "linkcalwidget.h"
#include "ui_linkcalwidget.h"

#include "linkcalculation.h"

#include "systemmargincalwidget.h"

LinkCalWidget::LinkCalWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkCalWidget)
{
    ui->setupUi(this);
    ui->toolBox->setCurrentIndex(0);

    m_linkCalculator = new LinkCalculation;
    ui->pgeSysMarginCal = new SystemMarginCalWidget;
}

LinkCalWidget::~LinkCalWidget()
{
    delete ui;
    delete m_linkCalculator;
}
void LinkCalWidget::on_btnAntGainCal_clicked()
{
    float antennaDiameter = ui->leAntDiameter->text().toFloat();
    float frequency = ui->leAntGainFreq->text().toFloat();
    float efficienct = ui->leAntEfficiency->text().toFloat();

    float antennaGain = m_linkCalculator->antennaGain(antennaDiameter, frequency, efficienct);
    ui->textEdit->append(QString("天线增益         %1dB").arg(antennaGain));
}

void LinkCalWidget::on_btnFreeSpaceLossCal_clicked()
{
    float distance = ui->leFreeSpaceDis->text().toFloat();
    float frequency = ui->leFreeSpaceFreq->text().toFloat();

    float loss = m_linkCalculator->freeSpaceTransmissionLoss(frequency, distance);
    ui->textEdit->append(QString("自由空间损失     %1dB").arg(loss));
}

void LinkCalWidget::on_btnInfoRateCal_clicked()
{
    float transRate = ui->leR24R1->text().toFloat();
    float FEC = ui->leNumeratorFEC4R1->text().toFloat() / ui->leDenominatorFEC4R1->text().toFloat();
    float RS = ui->leNumeratorRS4R1->text().toFloat() / ui->leDenominatorRS4R1->text().toFloat();

    float infoRate = m_linkCalculator->informationRate(transRate, FEC, RS);
    ui->textEdit->append(QString("信息速率         %1Mbps").arg(infoRate));
}

void LinkCalWidget::on_btnTransRateCal_clicked()
{
    float infoRate = ui->leR14R2->text().toFloat();
    float FEC = ui->leNumeratorFEC4R2->text().toFloat() / ui->leDenominatorFEC4R2->text().toFloat();
    float RS = ui->leNumeratorRS4R2->text().toFloat() / ui->leDenominatorRS4R2->text().toFloat();

    float transRate = m_linkCalculator->transmissionRate(infoRate, FEC, RS);
    ui->textEdit->append(QString("传输速率         %1Mbps").arg(transRate));
}

void LinkCalWidget::on_btnSymRateCal_clicked()
{
    float transRate = ui->leR24R3->text().toFloat();
    int M = ui->leM4R3->text().toInt();

    float symbolRate = m_linkCalculator->symbolRate(transRate, M);
    ui->textEdit->append(QString("码率             %1MBaud").arg(symbolRate));
}

void LinkCalWidget::on_btnSysBwCal_clicked()
{
    float symbolRate = ui->leR34BW->text().toFloat();
    float alpha = ui->leAlpha4BW->text().toFloat();
    float Bs = ui->leBs4BW->text().toFloat();

    float bandwidth = m_linkCalculator->systemBandwidth(symbolRate, alpha, Bs);
    ui->textEdit->append(QString("带宽             %1MHz").arg(bandwidth));
}

void LinkCalWidget::on_btnAngleCal_clicked()
{
    float earthRadius = ui->leEarthRadius->text().toFloat();
    float satelliteAltitude = ui->leSatelliteAltitude->text().toFloat();
    float longitudeUnderSatellite = ui->leLongitudeUnderSatellite->text().toFloat();
    float longitudeOfEarthStation = ui->leLongitudeOfEarthStation->text().toFloat();
    float latitudeOfEarthStation = ui->leLatitudeOfEarthStation->text().toFloat();

    float azimuth = m_linkCalculator->azimuth(latitudeOfEarthStation, longitudeOfEarthStation, longitudeUnderSatellite);
    float pitchAngle = m_linkCalculator->pitchAngle(latitudeOfEarthStation, longitudeOfEarthStation,
                                                    longitudeUnderSatellite, earthRadius, satelliteAltitude);
    float distance = m_linkCalculator->distanceBetweenStationAndSatellite(latitudeOfEarthStation,
                                                                          longitudeOfEarthStation,
                                                                          longitudeUnderSatellite,
                                                                          earthRadius, satelliteAltitude);
    ui->textEdit->append(QString("俯仰角度 %1°, 方位角度 %2°,星与地站相距 %3km").arg(azimuth).arg(pitchAngle).arg(distance));
}

void LinkCalWidget::on_btnEquivalentNoiseCal_clicked()
{
    float Ter = ui->leTer->text().toFloat();
    float Ta = ui->leTa->text().toFloat();
    float T1 = ui->leT1->text().toFloat();
    float Lf = ui->leLf->text().toFloat();

    float equivalentNoise = m_linkCalculator->systemEquivalentNoise(Ter, Ta, T1, Lf);
    ui->textEdit->append(QString("等效噪音         %1K").arg(equivalentNoise));
}

void LinkCalWidget::on_btnInputFallbackIPBOesCal_clicked()
{
    float IPBO = ui->leIPBO->text().toFloat();
    float forwarderBw = ui->leForwarderBw->text().toFloat();
    float occupiedBw = ui->leOccupiedBw->text().toFloat();

    float inputFallbackIPBOes = m_linkCalculator->inputFallbackIPBOes(IPBO, forwarderBw, occupiedBw);
    ui->textEdit->append(QString("输入回退IPBOes   %1dB").arg(inputFallbackIPBOes));
}

void LinkCalWidget::on_btnUploadEIRPesCal_clicked()
{
    float IPBOes = ui->leIPBOes->text().toFloat();
    float SFD = -ui->leSFD->text().toFloat();
    float Lu = ui->leLu->text().toFloat();
    float uploadFreq = ui->leUploadFreq->text().toFloat();

    float uploadEIRPes = m_linkCalculator->EIRPes(IPBOes, SFD, Lu, uploadFreq);
    ui->textEdit->append(QString("上行EIRPes       %1dBW").arg(uploadEIRPes));
}

