#ifndef BACKGROUNDCLASSIC_H
#define BACKGROUNDCLASSIC_H

#include "Background.h"

namespace Ui
{
    class BackgroundClassic : public Ui::Background
    {
    public:
        BackgroundClassic();
        virtual void draw(QPainter &painter, const QRect &rect);
        void setBrush(const QBrush &brush);
        const QBrush& getBrush() const;

    private:
        QBrush brush;
    };
}

#endif // BACKGROUNDCLASSIC_H