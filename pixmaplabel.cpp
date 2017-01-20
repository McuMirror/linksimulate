#include "pixmaplabel.h"

PixmapLabel::PixmapLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    initialize();
}

PixmapLabel::~PixmapLabel()
{

}

void PixmapLabel::setPosition(QPointF pos) {
    if(pos.x() != m_position.x() || pos.y() != m_position.y()) {
        m_position = pos;
        emit positionChanged();
    }
}

QPointF PixmapLabel::position() const {
    return m_position;
}

void PixmapLabel::updatePosition()
{
    this->setGeometry(m_position.x() - this->geometry().width() / 2.f,
                      m_position.y() - this->geometry().height() / 2.f,
                      this->geometry().width(),
                      this->geometry().height());
}

void PixmapLabel::initialize()
{
    connect(this, SIGNAL(positionChanged()), this, SLOT(updatePosition()));
}
