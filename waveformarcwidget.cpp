#include "waveformarcwidget.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include <QDebug>

#define RadianceFromDegree(x) ((x) / 180.f * M_PI)

WaveformArcWidget::WaveformArcWidget(QWidget *parent) : QWidget(parent)
{
    connect(this, SIGNAL(positionChanged()), this, SLOT(updatePosition()));
    m_arcNum = 5;
    m_startArc = 0;
    m_endArc = 0;
    m_arcAngle = 30.f;
    m_innerRadius = 15.f;
    m_currentDirection = 0;
    m_shouldDrawArcs = true;
    m_arcColor = Qt::red;
}

void WaveformArcWidget::setupArcs()
{
    float areaWidth = this->width() < this->height() ? this->width() : this->height();

    float stepLength = (areaWidth / 2.f - m_innerRadius) / m_arcNum;

    for (int i = 0; i < m_arcNum; i++) {
        float radius = m_innerRadius + (i + 1) * stepLength;
        QPainterPath path;
        path.moveTo(radius * qCos(RadianceFromDegree(-m_arcAngle / 2)),
                    -radius * qSin(RadianceFromDegree(-m_arcAngle / 2)));
        path.arcTo(-radius, -radius, radius * 2, radius * 2,
                   -m_arcAngle / 2, m_arcAngle);
        m_paths.push_back(path);
    }
}

void WaveformArcWidget::startAnimation()
{
    m_timerID = startTimer(100);
    m_shouldDrawArcs = true;
}

QPointF WaveformArcWidget::position() const
{
    return m_position;
}

void WaveformArcWidget::setPosition(const QPointF &position)
{
    if(position.x() != m_position.x() || position.y() != m_position.y()) {
        m_position = position;
        emit positionChanged();
    }
}

void WaveformArcWidget::updatePosition() {
    this->setGeometry(m_position.x() - this->geometry().width() / 2.f,
                      m_position.y() - this->geometry().height() / 2.f,
                      this->geometry().width(),
                      this->geometry().height());
}

void WaveformArcWidget::updateTarget()
{

}

void WaveformArcWidget::stopAnimation()
{
    killTimer(m_timerID);
    m_shouldDrawArcs = false;
}

void WaveformArcWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.translate(this->rect().center().x(), this->rect().center().y());
    painter.rotate(m_currentDirection / M_PI * 180);

    if (m_shouldDrawArcs && m_paths.size() != 0) {
        QPen pen(m_arcColor);
        pen.setCapStyle(Qt::RoundCap);
        pen.setWidthF(1);
        painter.setPen(pen);

        for (int i = m_startArc; i <= m_endArc; i++) {
            painter.drawPath(m_paths[i]);
        }
    }
}

void WaveformArcWidget::timerEvent(QTimerEvent *event)
{
    // Logic error may happen
    if (m_startArc == 0 && m_endArc < m_arcNum - 1) {
        m_endArc++;
    } else if (m_endArc == m_arcNum - 1 && m_startArc < m_endArc) {
        m_startArc++;
    } else {
        m_startArc = 0;
        m_endArc = 0;
    }

    if (qAbs(m_target.y() - m_position.y()) <= 0.00001f)
        m_currentDirection = 0.f;
    else
        m_currentDirection = M_PI / 2 - qAtan((m_target.x() - m_position.x()) / (m_target.y() - m_position.y()));

    this->update();
}

QColor WaveformArcWidget::arcColor() const
{
    return m_arcColor;
}

void WaveformArcWidget::setArcColor(const QColor &arcColor)
{
    m_arcColor = arcColor;
}

QPointF WaveformArcWidget::target() const
{
    return m_target;
}

void WaveformArcWidget::setTarget(const QPointF &target)
{
    m_target = target;
}

float WaveformArcWidget::innerRadius() const
{
    return m_innerRadius;
}

void WaveformArcWidget::setInnerRadius(float innerRadius)
{
    m_innerRadius = innerRadius;
}



