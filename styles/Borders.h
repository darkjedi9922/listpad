#ifndef BORDER_H
#define BORDER_H

#include <QPainter>
#include "Sides.h"

namespace Ui
{
    class Borders
    {
    public:
        Borders();
        void draw(QPainter &painter, const QRect &rect);

        void setLengths(const Sides<int> &lengths);
        void setBrushes(const Sides<QBrush> &brushes);

        const Sides<int> &getLengths() const;
        const Sides<QBrush> &getBrushes() const;

    private:
        Sides<int> lengths;
        Sides<QBrush> brushes;
    };
}

#endif // BORDER_H
