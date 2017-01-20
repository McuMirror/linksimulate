#ifndef ITEMLABEL_H
#define ITEMLABEL_H


#include <QLabel>

class PixmapLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

public:
    PixmapLabel(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~PixmapLabel();
    void setPosition(QPointF pos);
    QPointF position() const;

public slots:
    void updatePosition();

signals:
    void positionChanged();

private:
    void initialize();

    QPointF         m_position;
};

#endif // ITEMLABEL_H
