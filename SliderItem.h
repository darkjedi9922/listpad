#ifndef SLIDERSLIDER_H
#define SLIDERSLIDER_H

#include <QWidget>

class SliderItem : public QWidget
{
public:
    explicit SliderItem(QWidget *parent = nullptr);

    void setBrush(const QBrush &);
    QBrush getBrush() const;
};

#endif // SLIDERSLIDER_H
