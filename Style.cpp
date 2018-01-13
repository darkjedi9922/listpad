#include "Style.h"

Style::Style() {}
void Style::setBackground(const QBrush &brush)
{
    backrgound = brush;
}
const QBrush& Style::getBackground() const
{
    return backrgound;
}
void Style::setText(const QColor &color)
{
    text = color;
}
const QColor& Style::getText() const
{
    return text;
}