#include "rscc_rs_parawidget.h"
#include "ui_rscc_rs_parawidget.h"
#include "rsccgenerator.h"
#include "rsgenerator.h"
#include <QMessageBox>


#include <QDebug>

RSCC_RS_ParaWidget::RSCC_RS_ParaWidget(EncodingParaWidget *parent) :
    EncodingParaWidget(parent),
    ui(new Ui::RSCC_RS_ParaWidget)
{
    ui->setupUi(this);
}

RSCC_RS_ParaWidget::~RSCC_RS_ParaWidget()
{
    delete ui;
}

Waveform RSCC_RS_ParaWidget::outputWithWaveform(const Waveform &inputWfm,int a)
{

    //RS编码部分
    Waveform rsWfm;
    size_t I,E,orDatalength;
        if(ui->cbxRSType->currentIndex() == 0){
            rsWfm = inputWfm;
        }
        else if(a == 1) {
                if (ui->cbxRSType->currentIndex() == 1){
                orDatalength=223;//截出的原始信息长度
                E = 16;//纠错能力，能纠 E 个错误
            }
            else if (ui->cbxRSType->currentIndex() == 2){
                orDatalength=239;
                E = 8;
            }

            if (ui->cbxRSDepth->currentIndex() == 0){
                I = 1;//交织深度
            }
            else if (ui->cbxRSDepth->currentIndex() == 1){
                I = 2;//交织深度
            }
            else if (ui->cbxRSDepth->currentIndex() == 2){
                I = 3;//交织深度
            }
            else if (ui->cbxRSDepth->currentIndex() == 3){
                I = 4;//交织深度
            }
            else if (ui->cbxRSDepth->currentIndex() == 4){
                I = 5;//交织深度
            }
            else if (ui->cbxRSDepth->currentIndex() == 5){
                I = 8;//交织深度
            }

            RSGenerator rsgenerator(I,E,orDatalength);//分别为 I交织深度 E纠错能力 orDatalength截出的原始信息长度
            rsWfm = rsgenerator.outputWithInputWfm(inputWfm);
        } else {
            rsWfm = inputWfm;
        }

    //卷积编码
    Waveform rsccWfm;
        bool OK = false;
        size_t k=1, n=2, m ,rsccType , rsccPhase;//2017.1.10 by luhang

        if(ui->cbxRSCCType->currentIndex() == 0 ){
           rsccWfm = rsWfm;
        }
        else if(a == 2){
            if (ui->cbxRSCCType->currentIndex() == 1){
               rsccType = 1;
            }else if (ui->cbxRSCCType->currentIndex() == 2 ){
               rsccType = 2;
            }

            if (ui->cbxRSCCPhase->currentIndex() == 0){
               rsccPhase = 0;
            } else if (ui->cbxRSCCPhase->currentIndex() == 1) {
               rsccPhase = 1;
            } else if (ui->cbxRSCCPhase->currentIndex() == 2) {
               rsccPhase = 2;
            } else if (ui->cbxRSCCPhase->currentIndex() == 3) {
               rsccPhase = 3;
            }

            m = ui->leMValue->text().toUInt(&OK, 10);
            if(!OK)
                throw std::exception("leMValue error");

            std::vector<char16_t> g(n * k, 0);

            if(!getG(g))
                throw std::exception("getG error");

            RSCCGenerator generator(n, k, m, rsccType,rsccPhase,g.data());
            rsccWfm = generator.outputWithInputWfm(rsWfm);

        }else{
            rsccWfm = rsWfm;
        }
    return rsccWfm;
}

bool RSCC_RS_ParaWidget::getG(std::vector<char16_t> &g)
{
    QStringList list = ui->leGs->text().split(",");

    size_t count = list.length();

    if (g.size() != count)
        return false;

    bool OK;
    for(int i = 0; i < count; i++) {
        QString strG = list[i];
        OK = false;
        char16_t aG = strG.toUInt(&OK, 16);

        if(OK)
        {
            g[i] = aG;
            qDebug()<<"g[i]"<<g[i];
        } else
        {
            break;
        }
    }

    return OK;
}
