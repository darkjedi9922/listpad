#ifndef SLIDERSLIDER_H
#define SLIDERSLIDER_H

#include <QWidget>

class SliderItem : public QWidget
{
public:
    explicit SliderItem(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *);
};

#endif // SLIDERSLIDER_H
