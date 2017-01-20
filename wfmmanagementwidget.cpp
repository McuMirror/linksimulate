#include "wfmmanagementwidget.h"
#include "ui_wfmmanagementwidget.h"
#include "wfmimportdialog.h"
#include <QFileDialog>
#include "waveformmanager.h"
#include <QTableWidgetItem>

#include <QDebug>

WfmManagementWidget::WfmManagementWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WfmManagementWidget)
{
    ui->setupUi(this);

    int nameColWidth = 189;
    int lengthColWidth = 70;
    ui->tableWidget->setColumnWidth(0, nameColWidth);
    ui->tableWidget->setColumnWidth(1, lengthColWidth);

    connect(WaveformManager::getInstance(), SIGNAL(dataChanged()), this, SLOT(updateTableWidget()));
}

WfmManagementWidget::~WfmManagementWidget()
{
    delete ui;
}

void WfmManagementWidget::on_btnImport_clicked()
{
    WfmImportDialog wid;
    wid.exec();
}

void WfmManagementWidget::updateTableWidget()
{
    ui->tableWidget->clearContents();
    int row = 0, rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(++rowCount);

    WaveformMap::const_iterator iter = WaveformManager::getInstance()->sharedWaveform().cbegin();
    for(; iter != WaveformManager::getInstance()->sharedWaveform().cend(); iter++) {
        WaveformPtr pWfm = (*iter).second;
        QString name(pWfm.get()->name().c_str());
        QString length = QString::number(pWfm.get()->length());
        ui->tableWidget->setItem(row, 0, new  QTableWidgetItem(name));
        ui->tableWidget->item(row, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 1, new  QTableWidgetItem(length));
        ui->tableWidget->item(row, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        row++;
    }
}

void WfmManagementWidget::on_btnExport_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                QString::fromUtf8("保存"),
                                                "",
                                                "MAT Files(*.mat);;All Files(*.*)");

    if (!path.isNull()) {
        for (int i = 0; i < path.length(); i++) {
            if (path[i] == '/')
                path.replace(i, 1, "\\");
        }
        WaveformList aList("");
        for (int i = 0; i < ui->tableWidget->selectedItems().size(); i += 2) {
            QString wfmName(ui->tableWidget->selectedItems()[i]->text());
            aList.addWaveform(WaveformManager::getInstance()->waveformWithName(wfmName.toStdString()));
        }
        WaveformManager::getInstance()->saveToMAT(aList, path.toStdString());
    }
}
