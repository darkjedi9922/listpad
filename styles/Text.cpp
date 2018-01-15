#include "Text.h"
using namespace Ui;

Text::Text() {}
Text::~Text() {}

void Text::draw(QPainter &painter, const QRect &rect, const QString &text, int flags)
{
    painter.setPen(color);
    painter.setFont(font);
    painter.drawText(rect, flags, text);
}
QSize Text::size(const QString &text, int flags)
{
    return QFontMetrics(font).size(flags, text);
}
