#include "MenuButton.h"
#include <QPainter>

MenuButton::MenuButton(QWidget *parent) : QPushButton(parent),
    hoverBackgroundBrush(QBrush(QColor("#383838"))),
    textColor(QColor("#c5c5c5")),
    textFont(QFont("Arial", 12, 100)),
    normalIcon(QPixmap("images/folder.png"))
{
    hover = false;
    setFixedHeight(36);
}
void MenuButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    // Фон
    if (hover) {
        painter.setBrush(hoverBackgroundBrush);
        painter.drawRect(rect());
    }

    // Иконка
    painter.drawPixmap(18, rect().center().y() - normalIcon.height() / 2, normalIcon.width(), normalIcon.height(), normalIcon);

    // Текст
    painter.setPen(textColor);
    painter.setFont(textFont);
    painter.drawText(18 + normalIcon.width() + 9, 0, width() - 18 - normalIcon.width() - 9, height(), Qt::AlignVCenter, text());
}
void MenuButton::enterEvent(QEvent *)
{
    hover = true;
    repaint();
}
void MenuButton::leaveEvent(QEvent *)
{
    hover = false;
    repaint();
}
