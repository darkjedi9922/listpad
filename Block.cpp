#include "Block.h"
#include <QPainter>

Block::Block(QWidget *parent) : QWidget(parent),
    background(QBrush(QColor("#e4e4e4"))),
    backgroundRadius(QPoint(10, 10))
{}

void Block::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(background);
    painter.drawRoundedRect(rect(), backgroundRadius.x(), backgroundRadius.y());
}
void Block::mousePressEvent(QMouseEvent *)
{
    emit mousePressed();
}
