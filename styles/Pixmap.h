#ifndef PIXMAP_H
#define PIXMAP_H

#include <QPainter>

namespace Ui
{
    class Pixmap
    {
    public:
        Pixmap();
        virtual ~Pixmap();
        void draw(QPainter &painter, const QRect &rect);

        QPixmap pixmap;
    };
}

#endif // PIXMAP_H
