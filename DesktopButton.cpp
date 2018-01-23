#include "DesktopButton.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>

DesktopButton::DesktopButton(QWidget *parent) : QPushButton(parent),
    borderRadius(QPoint(10, 10)),
    textFont(QFont("Arial", 10)),
    normalBackground(QBrush(QColor("#003366"))),
    normalTextColor(QColor("#e4e4e4")),
    hoverBackgroundPartOne(QPixmap(":images/button_blue/btn-hlb.png")),
    hoverBackgroundPartTwo(QPixmap(":images/button_blue/btn-hb.png")),
    hoverBackgroundPartThree(QPixmap(":images/button_blue/btn-hrb.png")),
    hoverTextColor(QColor("#249dcd"))
{
    hovered = false;

    // Тень
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor(QColor(Qt::black));
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setEnabled(true);
    setGraphicsEffect(shadowEffect);
}

QSize DesktopButton::sizeHint() const
{
    return QSize(18 + QFontMetrics(textFont).width(text()) + 18, hoverBackgroundPartTwo.height());
}

void DesktopButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setFont(textFont);

    if (!isEnabled() || hovered)
    {
        graphicsEffect()->setEnabled(false);

        // Левый фон
        painter.drawPixmap(0, 0, hoverBackgroundPartOne);

        // Средний фон
        painter.setBrush(hoverBackgroundPartTwo);
        painter.drawRect(hoverBackgroundPartOne.width(), 0,
                         width() - hoverBackgroundPartOne.width() - hoverBackgroundPartThree.width(),
                         hoverBackgroundPartTwo.height());

        // Правый фон
        painter.drawPixmap(width() - hoverBackgroundPartThree.width(), 0, hoverBackgroundPartThree);

        // Текст
        painter.setPen(hoverTextColor);
        painter.setBrush(Qt::NoBrush);
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
    else
    {
        graphicsEffect()->setEnabled(true);

        // Фон
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(normalBackground);
        painter.drawRoundedRect(rect(), borderRadius.x(), borderRadius.y());
        painter.setRenderHint(QPainter::Antialiasing, false);

        painter.setPen(normalTextColor);
        painter.setBrush(Qt::NoBrush);
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
}

void DesktopButton::enterEvent(QEvent *)
{
    hovered = true;
    repaint();
}

void DesktopButton::leaveEvent(QEvent *)
{
    hovered = false;
    repaint();
}
