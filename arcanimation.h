#ifndef ARCANIMATION_H
#define ARCANIMATION_H


#include <QPropertyAnimation>
#include <QPainterPath>

// sweep angle not great than 180 degrees

class QPainterPath;

class ArcAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    ArcAnimation(QObject *target, const QByteArray &prop);

    QPointF center() const;
    void setCenter(const QPointF &center);

    float radius() const;
    void setRadius(float radius);

signals:
    void progressUpdated(float p);

protected:
    void updateCurrentTime(int currentTime);

private:
    QPointF             m_center;
    float               m_radius;
    QPainterPath        m_path;
};

#endif // ARCANIMATION_H
