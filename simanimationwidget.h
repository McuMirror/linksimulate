#ifndef SIMANIMATIONWIDGET_H
#define SIMANIMATIONWIDGET_H

#include <QWidget>

class PixmapLabel;
class ArcAnimation;
class WaveformArcWidget;

class SimAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SimAnimationWidget(QWidget *parent = 0);
    ~SimAnimationWidget();
    void startAnimation();
    void setup();

signals:
    void animationProgressUpdated(float p);

private slots:
    void updateAnimationProgress(float p);

public slots:

private:
    int                     m_duration;
    float                   m_satWidth;
    PixmapLabel *           m_satLabel;
    PixmapLabel *           m_earthLabel;
    PixmapLabel *           m_aerialLabel;
    WaveformArcWidget *     m_satArcs;
    WaveformArcWidget *     m_aerialArcs;
    ArcAnimation *          m_animSatLabel;
    ArcAnimation *          m_animSatArcs;
};

#endif // SIMANIMATIONWIDGET_H
