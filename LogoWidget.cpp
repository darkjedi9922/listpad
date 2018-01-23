#include "LogoWidget.h"
#include <QPainter>

LogoWidget::LogoWidget(QWidget *parent) :
    QWidget(parent),
    icon(QPixmap(":images/logo.png"))
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}
QSize LogoWidget::sizeHint() const
{
    return icon.size() + icon.size() / 2;
}

void LogoWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect().center().x() - icon.width() / 2,
                       rect().center().y() - icon.height() / 2,
                       icon);
}