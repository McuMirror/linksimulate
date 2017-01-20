#include "wfmimportdialog.h"
#include "ui_wfmimportdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include "waveformmanager.h"
#include "pncode.h"

#include <QDebug>

//#include <QTextStream>
//QTextStream cout(stdout, QIODevice::WriteOnly);//默认有序数列输出测试

WfmImportDialog::WfmImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WfmImportDialog)
{
    ui->setupUi(this);

    m_radioButtonGroup = new QButtonGroup();
    m_radioButtonGroup->addButton(ui->rdoPNCode, 1);
    m_radioButtonGroup->addButton(ui->rdoOrderedSeq, 2);
    m_radioButtonGroup->addButton(ui->rdoLocalFile, 3);

    ui->rdoPNCode->setChecked(true);
    ui->widgetPNCode->setEnabled(true);
    ui->widgetOrderedSeq->setEnabled(false);
    ui->widgetLocalFile->setEnabled(false);

    connect(ui->rdoPNCode, SIGNAL(clicked()), this, SLOT(switchRadioButton()));
    connect(ui->rdoOrderedSeq, SIGNAL(clicked()), this, SLOT(switchRadioButton()));
    connect(ui->rdoLocalFile, SIGNAL(clicked()), this, SLOT(switchRadioButton()));

    ui->ledWfmName->setFocus();
}

WfmImportDialog::~WfmImportDialog()
{
    delete ui;
    delete m_radioButtonGroup;
}

void WfmImportDialog::switchRadioButton()
{
    ui->widgetPNCode->setEnabled(false);
    ui->widgetOrderedSeq->setEnabled(false);
    ui->widgetLocalFile->setEnabled(false);

    switch (m_radioButtonGroup->checkedId()) {
    case 1:
        ui->widgetPNCode->setEnabled(true);
        break;
    case 2:
        ui->widgetOrderedSeq->setEnabled(true);
        break;
    case 3:
        ui->widgetLocalFile->setEnabled(true);
        break;
    }
}

void WfmImportDialog::on_btnConfirm_clicked()
{
   // ui->txtWfmData->setPlainText("1000000100100110100");

    try {
        switch (m_radioButtonGroup->checkedId()) {
        case 1: {
            bool orderOk, eigenOk;
            PNCode pncode(ui->ledWfmName->text().toStdString().c_str(),
                          ui->ledOrder->text().toInt(&orderOk),
                          ui->ledEigenpolynomial->text().toInt(&eigenOk, 16) >> 1);
            if (orderOk && eigenOk)
                WaveformManager::getInstance()->addWaveform(pncode);
            else
                throw std::exception("Error - Parameters input error, please check your input.");
            break;
        }
        case 2: {
            if (ui->txtWfmData->toPlainText().length() == 0)
            {
                int D_num1 = 0;
                QString numdefault1 = QString("%1").arg(D_num1,8,2,QLatin1Char('0'));
                QString numdefault = numdefault1;
                D_num1 ++;

               for(int kk=1;kk<128;kk++)
               {
                  numdefault1 = QString("%1").arg(D_num1,8,2,QLatin1Char('0'));
                  numdefault = numdefault + numdefault1;
                   D_num1++;
                //cout<<numdefault1<<endl;
                }
              // cout<<numdefault<<endl;

                ui->txtWfmData->setPlainText(numdefault);
                    //throw std::exception("Error - Waveform data shouldn't null, please check your input.");
                }
                       int radix = 2;
                       bool isOk;
                       QString dataString;
                       for (int i = 0; i < ui->txtWfmData->toPlainText().length(); i++) {
                           QString section;
                           QString aNumStr(ui->txtWfmData->toPlainText()[i]);
                           section.setNum(aNumStr.toInt(&isOk, radix));

                           if (isOk) {
                               dataString.append(section);
                           } else
                               throw std::exception("Error - Parameters input error, please check your input.");
                       }

                       float *data = new float[dataString.length()];
                       for (int i = 0; i < dataString.length(); i++) {
                           data[i] = dataString[i].toLatin1() - 48;
                       }

                       Waveform orderedSeq(ui->ledWfmName->text().toStdString().c_str(),
                                           dataString.length(),
                                           data);
                       WaveformManager::getInstance()->addWaveform(orderedSeq);
                       delete [] data;
                       break;
        }
        case 3: {
            if (!m_pathMATFile.isNull()) {
                WaveformList list = WaveformManager::getInstance()->loadFromMAT(m_pathMATFile.toStdString());
                for (int i = 0; i < list.count(); i++)
                    WaveformManager::getInstance()->addWaveform(list.waveformWithIndex(i));
            }
            break;
        }
        }
        this->close();
    }
    catch(std::exception e) {
        QMessageBox *mb = new QMessageBox(QMessageBox::Warning,
                                          "Error",
                                          e.what(),
                                          QMessageBox::Ok);
        mb->show();
    }
}

void WfmImportDialog::on_btnCancel_clicked()
{
    this->close();
}

void WfmImportDialog::on_btnScan_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString::fromUtf8("打开"),
                                                "",
                                                "MAT Files(*.MAT);;All Files(*.*)",
                                                0);
    if (!path.isNull()) {
        for (int i = 0; i < path.length(); i++) {
            if (path[i] == '/')
                path.replace(i, 1, "\\");
        }
        m_pathMATFile = path;
        ui->ledFilePath->setText(path);
    }
}
