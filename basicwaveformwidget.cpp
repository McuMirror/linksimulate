#include "basicwaveformwidget.h"
#include "ui_basicwaveformwidget.h"

#include "waveformmanager.h"
#include "instrumentcontroller.h"
#include "encodingparawidget.h"
#include "rscc_rs_parawidget.h"
#include "ldpc_parawidget.h"
#include "encoder.h"
#include "modulator.h"
#include "scrambler.h"
#include "mutilpathfade.h"
#include "frameconstructor.h"
#include "polarizationinterference.h"
#include "tracingerror.h"
#include "normalizer.h"
#include "groupdelay.h"
#include <QString>
#include <QMessageBox>

#include <QDebug>

BasicWaveformWidget::BasicWaveformWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BasicWaveformWidget)
{
    ui->setupUi(this);

    ui->cbxWfmSel->setInsertPolicy(QComboBox::InsertAtTop);
    ui->cbxWfmSel->setCurrentIndex(-1);

    connect(WaveformManager::getInstance(), SIGNAL(dataChanged()), this, SLOT(updateComboBox()));

    m_currentEncodingParaWidget = new RSCC_RS_ParaWidget(ui->encodingParaWidget);
    ui->lytEncodingParaWidget->addWidget(m_currentEncodingParaWidget);

    ui->toolBox->setCurrentIndex(0);
}

BasicWaveformWidget::~BasicWaveformWidget()
{
    delete ui;
    // Life cycle of the instance variables in this class is same as the whole application,
    // so there is no need to release them.
    delete m_currentEncodingParaWidget;
}

void BasicWaveformWidget::on_btnSendWfm_clicked()
{

    try {
        QString originWfmName = m_selectedWfm;
        Waveform originWfm = WaveformManager::getInstance()->waveformWithName(originWfmName.toStdString());


              // 前加扰
              Waveform preScrambledWfm;
              if (ui->grbPreScrambling->isChecked()) {
                  bool eigenpolynomialOk, orderOk, initialPhaseOk;
                  char32_t eigenpolynomial = ui->lePreScramblingPoly->text().toInt(&eigenpolynomialOk, 16) >> 1;
                      if(!eigenpolynomialOk)
                          throw std::exception("Error - Pre scramble parameters error, please check your input.");

                  QString orderStr = ui->cbxPreScramblingType->currentText().mid(2);// PN+X
                  size_t order = orderStr.toInt(&orderOk, 10);
                  char32_t initialPhase = ui->lePreScramblingInitialPhase->text().toInt(&initialPhaseOk, 16);
                      if(!initialPhaseOk)
                          throw std::exception("Error - Pre scramble parameters error, please check your input.");

                   Scrambler scrambler(eigenpolynomial, order, initialPhase);

                   preScrambledWfm = scrambler.scramblingCodeWithInput(originWfm);
              } else
              {
                   preScrambledWfm = originWfm;
              }

              // 信道编码，准备改为单RS
                 Waveform encodedWfm;
                  if (ui->grbChannelEncoding->isChecked()) {
                      encodedWfm = m_currentEncodingParaWidget->outputWithWaveform(preScrambledWfm,1);
                  } else
                  {
                      encodedWfm = preScrambledWfm;
                  }

              //加帧头 12.7改对了
                    Waveform dataFrameWfm;
                    if (ui->grbFrameStructure->isChecked()) {
                       FrameConstructor frameConstructor;
                       bool frameLengthOk, frameHeaderLengthOk, syncWordOk=true;
                       size_t frameLength = ui->leFrameLength->text().toInt(&frameLengthOk, 10);
                           if (!frameLengthOk)
                               throw std::exception("Error - Data Frame parameters error1, please check your input.");

                       size_t headerLength = ui->leFrameHeaderLength->text().toInt(&frameHeaderLengthOk, 10);
                           if (!frameHeaderLengthOk)
                               throw std::exception("Error - Data Frame parameters error2, please check your input.");

                       std::string syncWord = ui->leSyncWord->text().toUpper().toStdString();
                       for (size_t i = 0; i < syncWord.size(); i++) {
                           if (!(syncWord[i] >= '0' &&  syncWord[i] <= '9' || syncWord[i] >= 'A' && syncWord[i] <= 'F')) {
                               syncWordOk = false;
                               break;
                           }
                       }
                           if (!syncWordOk)
                               throw std::exception("Error - Data Frame parameters error3, please check your input.");

                       frameConstructor.setFrameLength(frameLength);
                       frameConstructor.setHeaderLength(headerLength);
                       frameConstructor.setSyncWord(ui->leSyncWord->text());

                       dataFrameWfm = frameConstructor.buildDataFrame(encodedWfm);
                    } else
                    {
                       dataFrameWfm = encodedWfm;
                    }

              //准备添加单卷积
            Waveform encodedWfmTwo;
             if (ui->grbChannelEncoding->isChecked()) {
                 encodedWfmTwo = m_currentEncodingParaWidget->outputWithWaveform(dataFrameWfm,2);
             } else
             {
                 encodedWfmTwo = dataFrameWfm;
             }


              // 后加扰
                  Waveform postScrambledWfm;
                  if (ui->grbPostScrambling->isChecked()) {
                      bool eigenpolynomialOk, orderOk, initialPhaseOk;
                      char32_t eigenpolynomial = ui->lePostScramblingPoly->text().toInt(&eigenpolynomialOk, 16) >> 1;
                          if(!eigenpolynomialOk)
                              throw std::exception("Error - Post scramble parameters error, please check your input.");

                      QString orderStr = ui->cbxPostScramblingType->currentText().mid(2);          // PN+X
                      size_t order = orderStr.toInt(&orderOk, 10);
                      char32_t initialPhase = ui->lePostScramblingInitialPhase->text().toInt(&initialPhaseOk, 16);
                          if(!initialPhaseOk)
                              throw std::exception("Error - Post scramble parameters error, please check your input.");

                      Scrambler scrambler(eigenpolynomial, order, initialPhase);
                      postScrambledWfm = scrambler.scramblingCodeWithInput(encodedWfmTwo);
                  } else
                  {
                      postScrambledWfm = encodedWfmTwo;
                  }

          // 调制
              Waveform modulatedWfm;
              if (ui->grbModulation->isChecked()) {
                  Modulator modulator;
                  Modulator::Modulation modulation;
                  switch (ui->cbxModulationType->currentIndex()) {
                  case 0:
                      modulation = Modulator::PSK_B;
                      break;
                  case 1:
                      modulation = Modulator::PSK_Q;
                      break;
                  case 2:
                      modulation = Modulator::PSK_8;
                      break;
                  case 3:
                      modulation = Modulator::APSK_16;
                      break;
                  case 4:
                      modulation = Modulator::QAM_16;
                      break;
                  }

                  bool bitRateOk;
                  float bitRate = ui->leBitRate->text().toFloat(&bitRateOk);      // Mbps
                      if (!bitRateOk)
                          throw std::exception("Error - Modulating parameters error, please check your input.");

                  postScrambledWfm.setBitRate(bitRate);

                  bool IFOk, sampleRateOk, rolloffFactorOk, phaseNoiseOk, DopplerShiftOk;
                  float IF = ui->leIntermediateFreq->text().toFloat(&IFOk);//IF 中频频率，单位MHz
                      if (!IFOk)
                          throw std::exception("Error - Modulating parameters error, please check your input.");
                  modulator.setSampleRate(ui->leSampleRate->text().toFloat(&sampleRateOk));
                      if (!sampleRateOk)
                          throw std::exception("Error - Modulating parameters error, please check your input.");

                  float rateOfSample = ui->leSampleRate->text().toFloat();
                  InstrumentController::getInstance()->setSampleRate(rateOfSample);//控制设备的采样频率

                  modulator.setAlpha(ui->cbxRolloffFactor->currentText().toFloat(&rolloffFactorOk));
                      if (!rolloffFactorOk)
                          throw std::exception("Error - Modulating parameters error, please check your input.");

                  float DopplerShift, phaseNoise;

          //多普勒频移特性
                  if (ui->grbDopplerShift->isChecked()) {
                      DopplerShift = ui->leDopplerShift->text().toFloat(&DopplerShiftOk);
                  } else
                  {
                      DopplerShift = 0.f;
                      DopplerShiftOk = true;
                  }
                      if (!DopplerShiftOk)
                          throw std::exception("Error - Modulating parameters error, please check your input.");

                  modulator.setDopplerShift(DopplerShift);

          //相噪特性
                  if (ui->grbPhaseNoise->isChecked()) {
                      phaseNoise = ui->lePhaseNoise->text().toFloat(&phaseNoiseOk);
                  } else
                  {
                      phaseNoise = 0.f;
                      phaseNoiseOk = true;
                  }

                  modulator.setPhaseNoise(phaseNoise);
                      if (!phaseNoiseOk)
                          throw std::exception("Error - Modulating parameters error, please check your input.");

                  modulatedWfm = modulator.modulate(postScrambledWfm, modulation, IF);
              } else
              {
                  modulatedWfm = postScrambledWfm;
              }

              modulatedWfm.setSampleRate(ui->leSampleRate->text().toFloat());

          // 多径衰落
             Waveform multipathFadeWfm;
             if (ui->grbMultipathFade->isChecked()) {

                 bool gainsOk, delaysOk, pathCountOk;
                 std::vector<float> gains;
                 std::vector<float> delays;
                 QString gainsString = ui->leMultipathGains->text();
                 QStringList gainsList = gainsString.split(",");
                 for (size_t i = 0; i < gainsList.size(); i++) {
                     gains.push_back(gainsList[i].toFloat(&gainsOk));
                     if (!gainsOk)
                         throw std::exception("Error - Multipath gains parameters error, please check your input.");
                 }

                 QString delaysString = ui->leMultipathDelays->text();
                 QStringList delaysList = delaysString.split(",");
                 for (size_t i = 0; i < delaysList.size(); i++) {
                     delays.push_back(delaysList[i].toFloat(&delaysOk));
                     if (!delaysOk)
                         throw std::exception("Error - Multipath delays parameters error, please check your input.");
                 }

                 int pathCount = ui->leMultipathCount->text().toInt(&pathCountOk);
                 if (!pathCountOk || (pathCount != gains.size()) || (pathCount != delays.size()))
                     throw std::exception("Error - Multipath delays parameters error, please check your input.");

                 multipathFadeWfm = MultipathFade::multipathFadeWithGainAndDelay(modulatedWfm, gains, delays);
             } else
             {
                 multipathFadeWfm = modulatedWfm;
             }

    // 极化干扰
        Waveform polarInterferenceWfm;
        if (ui->grbPolarInterference->isChecked()) {
           bool gainOk, delayOk;
        float gain = ui->leInterferenceAmplitude->text().toFloat(&gainOk);
          if (!gainOk)
          throw std::exception("Error - Polarization interference simulation parameters error, please check your input.");

        float delay = ui->leInterferenceDelay->text().toFloat(&delayOk);
         if (!delayOk)
            throw std::exception("Error - Polarization interference simulation parameters error, please check your input.");
        polarInterferenceWfm = PolarizationInterference::polarizationInterference(multipathFadeWfm, gain, delay);
        } else
        {
           polarInterferenceWfm = multipathFadeWfm;
        }

    // 群时延
        Waveform groupDelayWfm;
        if (ui->grbGroupDelay->isChecked()) {
          bool gainsOk;
          std::vector<float> gains;
           QString gainsString = ui->leGroupDelay->text();
          QStringList gainsList = gainsString.split(",");
          for (size_t i = 0; i < gainsList.size(); i++) {
              gains.push_back(gainsList[i].toFloat(&gainsOk));
             if (!gainsOk)
                 throw std::exception("Error - Group delay parameters error, please check your input.");
          }
         groupDelayWfm = GroupDelay::addGroupDelay(gains,polarInterferenceWfm);
        } else {
         groupDelayWfm = polarInterferenceWfm;
        }

        // 归一化
        Normalizer normalizer;
        Waveform normalizedWfm = normalizer.normalizing(groupDelayWfm);


        //跟踪误差
        Waveform tracingErrorWfm;
                if (ui->grbTracingError->isChecked()) {
                bool gainErrorOk;
                float gainError = ui->leTracingError->text().toFloat(&gainErrorOk);
                if (!gainErrorOk)
                      throw std::exception("Error - Antenna tracing simulation parameters error, please check your input.");
                tracingErrorWfm = TracingError::addTracingError(normalizedWfm, gainError);
                } else {
                tracingErrorWfm = normalizedWfm;
                }


        Waveform finalWfm = tracingErrorWfm;
        WaveformManager::getInstance()->addWaveform(finalWfm);

        if (!ui->checkBox->isChecked())
            InstrumentController::getInstance()->sendWaveform(finalWfm);
    }
    catch(std::exception e) {
      QMessageBox *mb = new QMessageBox(QMessageBox::Warning,
                                          "Error",
                                          e.what(),
                                          QMessageBox::Ok);
        mb->show();
    }
}

void BasicWaveformWidget::updateComboBox()
{
    ui->cbxWfmSel->clear();

    WaveformMap::const_iterator iter = WaveformManager::getInstance()->sharedWaveform().cbegin();
    for(; iter != WaveformManager::getInstance()->sharedWaveform().cend(); iter++) {
        WaveformPtr pWfm = (*iter).second;
        QString name(pWfm.get()->name().c_str());
        ui->cbxWfmSel->addItem(name);
    }

    ui->cbxWfmSel->setCurrentIndex(-1);
}

void BasicWaveformWidget::on_cbxWfmSel_currentIndexChanged(const QString &arg1)
{
    m_selectedWfm = arg1;
}

void BasicWaveformWidget::on_cbxChannelSel_currentIndexChanged(const QString &arg1)
{
    m_selectedChannel = arg1;//有效的
    if(m_selectedChannel == "Channel 1")
          InstrumentController::chooseChannel(1);
    if(m_selectedChannel == "Channel 2")
          InstrumentController::chooseChannel(2);
    if(m_selectedChannel == "Both")
          InstrumentController::chooseChannel(3);
}

void BasicWaveformWidget::on_cbxEncodingType_currentIndexChanged(int index)
{
    ui->lytEncodingParaWidget->removeWidget(m_currentEncodingParaWidget);
    delete m_currentEncodingParaWidget;
    switch (index) {
    case 0:
        m_currentEncodingParaWidget = new RSCC_RS_ParaWidget(ui->encodingParaWidget);
        break;
    case 1:
        m_currentEncodingParaWidget = new LDPC_ParaWidget(ui->encodingParaWidget);// change to other encoding parameters widget later
        break;
    }
    ui->lytEncodingParaWidget->addWidget(m_currentEncodingParaWidget);
}

void BasicWaveformWidget::on_btnEncoding_clicked()
{
    ui->toolBox->setCurrentIndex(0);
}

void BasicWaveformWidget::on_btnModulating_clicked()
{
    ui->toolBox->setCurrentIndex(1);
}

void BasicWaveformWidget::on_btnSimulating_clicked()
{
    ui->toolBox->setCurrentIndex(2);
}
