#ifndef STYLE_H
#define STYLE_H

#include <QBrush>
#include <QColor>

class Style
{
public:
    Style();
    void setBackground(const QBrush &brush);
    const QBrush &getBackground() const;
    void setText(const QColor &color);
    const QColor &getText() const;

private:
    QBrush backrgound;
    QColor text;
};

#endif // STYLE_H
