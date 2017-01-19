#include "arcanimation.h"
#include <assert.h>
#include <QtCore/qmath.h>

ArcAnimation::ArcAnimation(QObject *target, const QByteArray &prop)
    : QPropertyAnimation(target, prop)
{

}

QPointF ArcAnimation::center() const
{
    return m_center;
}

void ArcAnimation::setCenter(const QPointF &center)
{
    m_center = center;
}
float ArcAnimation::radius() const
{
    return m_radius;
}

void ArcAnimation::setRadius(float radius)
{
    m_radius = radius;
}

void ArcAnimation::updateCurrentTime(int currentTime)
{
    if(m_path.isEmpty()) {
        // there are many bugs need to be fixed up
        //  i.e. radius means 0, value of k invalid
        QPointF to = endValue().toPointF();
        QPointF from = startValue().toPointF();

        float k1 = (m_center.y() - from.y()) /  (from.x() - m_center.x());
        float k2 = (m_center.y() - to.y()) /  (to.x() - m_center.x());
        float startAngle = qAtan(k1) / M_PI * 180;
        float endAngle = qAtan(k2) / M_PI * 180 + 180;

        m_path.moveTo(from);
        m_path.arcTo(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2,
                     startAngle, endAngle - startAngle);
        m_path.moveTo(from);
    }
    int dura = duration();
    const qreal progress = ((dura == 0) ? 1 : ((((currentTime - 1) % dura) + 1) / qreal(dura)));

    qreal easedProgress = easingCurve().valueForProgress(progress);
    if (easedProgress > 1.0) {
        easedProgress -= 1.0;
    } else if (easedProgress < 0) {
        easedProgress += 1.0;
    }
    QPointF pt = m_path.pointAtPercent(easedProgress);
    updateCurrentValue(pt);
    emit valueChanged(pt);

    emit progressUpdated(progress);
}



