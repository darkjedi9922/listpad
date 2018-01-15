#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QPainter>

namespace Ui
{
    class Background
    {
    public:
        Background();
        virtual void draw(QPainter &painter, const QRect &rect);

        void setBrush(const QBrush &brush);
        void setRadius(const QPoint &radius);

        const QBrush& getBrush() const;
        const QPoint& getRadius() const;

    private:
        QBrush brush;
        QPoint radius;
        QMargins borders;
    };
}

#endif // BACKGROUND_H
