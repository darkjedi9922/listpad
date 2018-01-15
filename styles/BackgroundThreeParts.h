#ifndef BACKGROUNDTHREEPARTS_H
#define BACKGROUNDTHREEPARTS_H

#include "BackgroundClassic.h"

namespace Ui
{
    class BackgroundThreeParts : public Background
    {
    public:
        BackgroundThreeParts();
        virtual ~BackgroundThreeParts();
        void draw(QPainter &painter, const QRect &rect);

        void setPartOne(const QBrush &brush);
        void setPartTwo(const QBrush &brush);
        void setPartThree(const QBrush &brush);
        void setPartOneLength(int length);
        void setPartThreeLength(int length);

        const QBrush& getPartOne() const;
        const QBrush& getPartTwo() const;
        const QBrush& getPartThree() const;
        int getPartOneLength() const;
        int getPartThreeLength() const;

    private:
        QBrush partOne;
        QBrush partTwo;
        QBrush partThree;
        int partOneLength;
        int partThreeLength;
    };
}

#endif // BACKGROUNDTHREEPARTS_H
