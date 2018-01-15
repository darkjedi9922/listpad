#include "MenuButton.h"
#include <QPainter>

MenuButton::MenuButton(QWidget *parent) : QPushButton(parent),
    hoverBackgroundBrush(QBrush(QColor("#383838"))),
    checkedBackgroundBrush(QColor("#0b0b0b")),
    textColor(QColor("#c5c5c5")),
    checkedTextColor(QColor("#979797")),
    textFont(QFont("Arial", 12, 100)),
    normalIcon(QPixmap("images/folder.png")),
    checkedIcon(QPixmap("images/checkedFolder.png"))
{
    hovered = false;
    setFixedHeight(36);
    setCheckable(true);
}

void MenuButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    // Фон
    if (isChecked()) painter.setBrush(checkedBackgroundBrush);
    else if (hovered) painter.setBrush(hoverBackgroundBrush);
    painter.drawRect(rect());

    // Иконка
    painter.drawPixmap(18, rect().center().y() - normalIcon.height() / 2,
                       normalIcon.width(), normalIcon.height(),
                       (isChecked() ? checkedIcon : normalIcon));

    // Текст
    if (isChecked()) painter.setPen(checkedTextColor);
    else painter.setPen(textColor);
    painter.setFont(textFont);
    painter.drawText(18 + normalIcon.width() + 9, 0, width() - 18 - normalIcon.width() - 9, height(), Qt::AlignVCenter, text());
}
void MenuButton::enterEvent(QEvent *)
{
    hovered = true;
    repaint();
}
void MenuButton::leaveEvent(QEvent *)
{
    hovered = false;
    repaint();
}
