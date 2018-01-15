#include "BackgroundClassic.h"
#include <QPainter>
using namespace Ui;

BackgroundClassic::BackgroundClassic() : Background(),
    brush(QBrush())
{}

void BackgroundClassic::draw(QPainter &painter, const QRect &rect)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    if (getRadius().isNull()) painter.drawRect(rect);
    else {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawRoundedRect(rect, getRadius().x(), getRadius().y());
        painter.setRenderHint(QPainter::Antialiasing, false);
    }
}

void BackgroundClassic::setBrush(const QBrush &brush)
{
    this->brush = brush;
}

const QBrush& BackgroundClassic::getBrush() const
{
    return brush;
}