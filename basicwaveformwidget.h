#ifndef BASICWAVEFORMWIDGET_H
#define BASICWAVEFORMWIDGET_H

#include <QWidget>

namespace Ui {
class BasicWaveformWidget;
}

class EncodingParaWidget;

class BasicWaveformWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BasicWaveformWidget(QWidget *parent = 0);
    ~BasicWaveformWidget();

private:
    Ui::BasicWaveformWidget *   ui;
    QString                     m_selectedWfm;
    QString                     m_selectedChannel;
    EncodingParaWidget *        m_currentEncodingParaWidget;

private slots:
    void on_btnSendWfm_clicked();
    void updateComboBox();
    void on_cbxWfmSel_currentIndexChanged(const QString &arg1);
    void on_cbxChannelSel_currentIndexChanged(const QString &arg1);
    void on_cbxEncodingType_currentIndexChanged(int index);


    void on_btnEncoding_clicked();
    void on_btnModulating_clicked();
    void on_btnSimulating_clicked();
};

#endif // BASICWAVEFORMWIDGET_H
