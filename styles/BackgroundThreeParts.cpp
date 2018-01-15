#include "BackgroundThreeParts.h"
using namespace Ui;

BackgroundThreeParts::BackgroundThreeParts()
{
    partOneLength = 0;
    partThreeLength = 0;
}
BackgroundThreeParts::~BackgroundThreeParts() {}
void BackgroundThreeParts::draw(QPainter &painter, const QRect &rect)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(partOne);
    painter.drawRect(QRect(rect.x(), rect.y(), partOneLength, rect.height()));
    painter.setBrush(partTwo);
    painter.drawRect(QRect(partOneLength, rect.y(), rect.width() - partOneLength - partThreeLength, rect.height()));
    painter.setBrush(partThree);
    painter.setBrushOrigin(rect.right() - partThreeLength + 1, 0);
    painter.drawRect(QRect(rect.right() - partThreeLength + 1, rect.y(), partThreeLength, rect.height()));
}
