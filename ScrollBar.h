#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QWidget>
#include <QBoxLayout>
#include "Slider.h"

class ScrollBar : public QWidget
{
    Q_OBJECT
public:
    ScrollBar(Qt::Orientation orientation, QWidget *parent = nullptr);
    void setOrientation(Qt::Orientation orientation);
    void setMinimum(int min);
    void setMaximum(int max);
    void setSingleStep(int step);

    int getMinimum();
    int getMaximum();
    Slider *getSlider() const;

    void setValue(int);
    int getValue() const;

private:
    QBoxLayout *layout;
    Slider *slider;
};

#endif // SCROLLBAR_H
