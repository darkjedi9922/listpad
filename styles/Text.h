#ifndef TEXT_H
#define TEXT_H

#include <QPainter>

namespace Ui
{
    class Text
    {
    public:
        Text();
        virtual ~Text();
        void draw(QPainter &painter, const QRect &rect, const QString &text, int flags = 0);
        QSize size(const QString &text, int flags = 0);

        QColor color;
        QFont font;
    };
}

#endif // TEXT_H
