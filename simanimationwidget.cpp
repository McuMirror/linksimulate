#include "simanimationwidget.h"
#include "pixmaplabel.h"
#include <QPainterPath>
#include "arcanimation.h"
#include "waveformarcwidget.h"
#include <QPainter>
#include <QtCore/qmath.h>

#include <QDebug>

SimAnimationWidget::SimAnimationWidget(QWidget *parent) : QWidget(parent)
{
    m_satLabel = new PixmapLabel(this);
    m_earthLabel = new PixmapLabel(this);
    m_aerialLabel = new PixmapLabel(this);
    m_satArcs = new WaveformArcWidget(this);
    m_aerialArcs = new WaveformArcWidget(this);
    m_animSatLabel = new ArcAnimation(m_satLabel, "position");;
    m_animSatArcs = new ArcAnimation(m_satArcs, "position");
    m_satWidth = 40.f;
    m_duration = 5000;

    connect(m_animSatArcs, SIGNAL(finished()), m_satArcs, SLOT(stopAnimation()));
    connect(m_animSatLabel, SIGNAL(progressUpdated(float)), this, SLOT(updateAnimationProgress(float)));

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(74, 183, 255));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

SimAnimationWidget::~SimAnimationWidget()
{
    delete m_satLabel;
    delete m_earthLabel;
    delete m_aerialLabel;
    delete m_animSatLabel;
    delete m_animSatArcs;
    delete m_aerialArcs;
    delete m_satArcs;
}

void SimAnimationWidget::startAnimation()
{
    m_animSatLabel->start();
    m_animSatArcs->start();
    m_satArcs->startAnimation();
//    m_aerialArcs->startAnimation();
}

void SimAnimationWidget::setup()
{
    QPointF satStart(this->width() - m_satWidth / 2.f, this->height() / 2.f);
    QPointF satEnd(m_satWidth / 2.f, this->height() / 2.f);

    QPixmap satPixmap(QString::fromLatin1(":/images/satellite.png"));
    m_satLabel->setGeometry(0, 0, m_satWidth, m_satWidth);
    m_satLabel->setPixmap(satPixmap.scaled(m_satWidth, m_satWidth));
    m_satLabel->setPosition(satStart);

    // Earth
    float earthWidth = this->width();
    float earthRadius = earthWidth / 2.f;
    QPixmap earthPixmap(QString::fromLatin1(":/images/earth.png"));
    m_earthLabel->setGeometry(0, this->height() - earthRadius / 3, earthWidth, earthWidth);
    m_earthLabel->setPixmap(earthPixmap.scaled(earthWidth, earthWidth));

    // Satlite
    QPointF earthCenter = m_earthLabel->geometry().center();
    float radius = qSqrt((earthCenter.x() - satStart.x()) * (earthCenter.x() - satStart.x())
                         + (earthCenter.y() - satStart.y()) * (earthCenter.y() - satStart.y()));
    m_animSatLabel->setCenter(QPointF(m_earthLabel->geometry().x() + earthRadius,
                                 m_earthLabel->geometry().y() + earthRadius));

    m_animSatLabel->setRadius(radius);
    m_animSatLabel->setStartValue(satStart);
    m_animSatLabel->setEndValue(satEnd);
    m_animSatLabel->setLoopCount(1);
    m_animSatLabel->setDuration(m_duration);

    // Waveform of satlite
    m_satArcs->setInnerRadius(m_satWidth);
    m_satArcs->setGeometry(0, 0, 200, 200);
    m_satArcs->setPosition(m_satLabel->position());
    m_satArcs->setTarget(QPointF(earthCenter.x(), earthCenter.y() - earthRadius));
    m_satArcs->setupArcs();
    m_satArcs->show();

    m_animSatArcs->setCenter(m_animSatLabel->center());
    m_animSatArcs->setRadius(radius);
    m_animSatArcs->setStartValue(satStart);
    m_animSatArcs->setEndValue(satEnd);
    m_animSatArcs->setLoopCount(1);
    m_animSatArcs->setDuration(m_duration);

    // Aerial
    float aerialWidth = earthWidth / 10;
    float aerialHeight = aerialWidth * 5 / 8.f;
    QPixmap aerialPixmap(QString::fromLatin1(":/images/aerial.png"));
    m_aerialLabel->setGeometry(0, 0, aerialWidth, aerialHeight);
    m_aerialLabel->setPosition(m_satArcs->target());
    m_aerialLabel->setPixmap(aerialPixmap.scaled(m_aerialLabel->width(), m_aerialLabel->height()));

    // Waveform of aerial
//    m_aerialArcs->setInnerRadius(aerialWidth);
//    m_aerialArcs->setGeometry(0, 0, 200, 200);
//    m_aerialArcs->setPosition(m_aerialLabel->position());
//    m_aerialArcs->setTarget(m_satLabel->position());
//    m_aerialArcs->setupArcs();
//    m_aerialArcs->show();
}

void SimAnimationWidget::updateAnimationProgress(float p)
{
    emit animationProgressUpdated(p);
}
