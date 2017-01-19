#include "calculationresultsdialog.h"
#include "ui_calculationresultsdialog.h"
#include "linkcalculation.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

//去掉了上面一行！
CalculationResultsDialog::CalculationResultsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalculationResultsDialog)
{
    ui->setupUi(this);
}

CalculationResultsDialog::~CalculationResultsDialog()
{
    delete ui;
}

void CalculationResultsDialog::setupCalculationResults(LinkCalculation *linkCalculator)
{
    ui->labInformationRate->setText(QString::number(linkCalculator->informationRate()));
    ui->labSymbolRate->setText(QString::number(linkCalculator->symbolRate()));
    ui->labSystemBandwidth->setText(QString::number(linkCalculator->systemBandwidth()));
    ui->labDistance->setText(QString::number(linkCalculator->distanceBetweenStationAndSatellite()));
    ui->labUplinkTransLoss->setText(QString::number(linkCalculator->uplinkTransmissionLoss()));
    ui->labDownlinkTransLoss->setText(QString::number(linkCalculator->downlinkTransmissionLoss()));
    ui->labUplinkTotalLoss->setText(QString::number(linkCalculator->uplinkTotalLoss()));
    ui->labDownlinkTotalLoss->setText(QString::number(linkCalculator->downlinkTotalLoss()));
    ui->labIPBOes->setText(QString::number(linkCalculator->IPBOes()));
    ui->labUplinkCOverTWithoutInterference->setText(QString::number(linkCalculator->uplinkCOverTWithoutInterference()));
    ui->labUplinkCOverNWithoutInterference->setText(QString::number(linkCalculator->uplinkCOverNWithoutInterference()));
    ui->labUplinkCOverTWithInterference->setText(QString::number(linkCalculator->uplinkCOverTWithInterference()));
    ui->labUplinkCOverNWithInterference->setText(QString::number(linkCalculator->uplinkCOverNWithInterference()));
    ui->labSystemEquivalentNoise->setText(QString::number(linkCalculator->systemEquivalentNoise()));
    ui->labAntennaDiameter->setText(QString::number(linkCalculator->antennaDiameter()));
    ui->labAntennaGain->setText(QString::number(linkCalculator->antennaGain()));
    ui->labReceiverAntennaGOverT->setText(QString::number(linkCalculator->receiverAntennaGOverT()));
    ui->labLinkTotalCOverT->setText(QString::number(linkCalculator->linkTotalCOverT()));
    ui->labLinkTotalCOverN->setText(QString::number(linkCalculator->linkTotalCOverN()));
    ui->labEbOverN0->setText(QString::number(linkCalculator->EbOverN0()));
    ui->labEbOverN0Margin->setText(QString::number(linkCalculator->EbOverN0Margin()));
    ui->labOPBOes->setText(QString::number(linkCalculator->OPBOes()));
    ui->labDownlinkCOverTWithoutInterference->setText(QString::number(linkCalculator->downlinkCOverTWithoutInterference()));
    ui->labDownlinkCOverNWithoutInterference->setText(QString::number(linkCalculator->downlinkCOverNWithoutInterference()));
    ui->labDownlinkCOverTWithInterference->setText(QString::number(linkCalculator->downlinkCOverTWithInterference()));
    ui->labDownlinkCOverNWithInterference->setText(QString::number(linkCalculator->downlinkCOverNWithInterference()));
}

void CalculationResultsDialog::on_btnConfirm_clicked()
{
    this->close();
}

void CalculationResultsDialog::on_btnExport_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                QString::fromUtf8("保存"),
                                                "",
                                                "Text Files(*.txt);;All Files(*.*)");

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

    QTextStream out(&file);
    out << QString("%1").arg(QString::fromUtf8("信息速率"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("符号速率"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("FEC"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("RS"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("调制系数"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("滚降系数"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("保护带宽"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("上行频率"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("下行频率"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("接收天线直径"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("天线效率"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
//    out << QString("%1").arg(QString::fromUtf8("接收机等效噪声"), 16, ' ')
//        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("接收天线噪声温度"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("环境温度"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
//    out << QString("%1").arg(QString::fromUtf8("馈线损耗"), 16, ' ')
//        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("经度"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("纬度"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("卫星高度"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("星下经度"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
//    out << QString("%1").arg(QString::fromUtf8("转发器带宽"), 16, ' ')
//        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("转发器输入功率回退"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("转发器输出功率回退"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("卫星天线G/T"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("转发器饱和通量密度SFD"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("卫星EIRP"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
//    out << QString("%1").arg(QString::fromUtf8("总干扰噪声"), 16, ' ')
//        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
//    out << QString("%1").arg(QString::fromUtf8("接收机Eb/N0门限值"), 16, ' ')
//        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("占用带宽"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("系统等效噪声"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("星地距离"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("上行传输衰减"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("下行传输衰减"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("总输入回退"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("总输出回退"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("上行C/T"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("上行C/N"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("下行C/T"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("下行C/N"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("链路总C/T"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("链路总C/N"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("接收天线G/T"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("链路Eb/N0"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;
    out << QString("%1").arg(QString::fromUtf8("链路Eb/N0余量"), 16, ' ')
        << QString("%1").arg(ui->labInformationRate->text(), 16, ' ') << QString("Mbps\n\r") << endl;

    file.close();
}
