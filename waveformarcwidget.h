#ifndef WAVEFORMARCWIDGET_H
#define WAVEFORMARCWIDGET_H

#include <QWidget>

class WaveformArcWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

public:
    explicit WaveformArcWidget(QWidget *parent = 0);
    void setupArcs();
    void startAnimation();

    QPointF position() const;
    void setPosition(const QPointF &position);

    float innerRadius() const;
    void setInnerRadius(float innerRadius);

    QPointF target() const;
    void setTarget(const QPointF &target);

    QColor arcColor() const;
    void setArcColor(const QColor &arcColor);

signals:
    void positionChanged();

public slots:
    void updatePosition();
    void updateTarget();
    void stopAnimation();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    QPointF                     m_position;
    QPointF                     m_target;
    QVector<QPainterPath>       m_paths;
    QColor                      m_arcColor;
    int                         m_arcNum;
    int                         m_startArc;
    int                         m_endArc;
    float                       m_innerRadius;
    float                       m_arcAngle;                 // In degree
    float                       m_currentDirection;         // In degree
    int                         m_timerID;
    bool                        m_shouldDrawArcs;
};

#endif // WAVEFORMARCWIDGET_H
