#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>
#include "SliderItem.h"

class Slider : public QWidget
{
    Q_OBJECT

signals:
    void valueChanged(int newValue);

public:
    Slider(Qt::Orientation orientation, QWidget *parent = 0);

    // orientation
    void setOrientation(Qt::Orientation);
    Qt::Orientation getOrientation() const;

    // steps
    void setSingleStep(int);
    int getSingleStep() const;

    // values
    void setMinimum(int);
    int getMinimum() const;
    void setMaximum(int);
    int getMaximum() const;
    void setValue(int);
    void setSliderPosition(int);

    // lengths
    void setSliderFixedLength(int);
    void setSliderAutoLength();
    int getSliderLength();
    int getLength();

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    void updateSliderItemSize();
    void updateSliderPosition();
    int calcSliderAutoLength();
    int calcRealValueFromPixel(int pxValue);
    int calcPixelValueFromReal(int realValue);
    int getOrientationValue(int hValue, int vValue);

    SliderItem *slider;
    Qt::Orientation orientation;
    int singleStep;
    int minimum;
    int maximum;
    int value;
    int sliderFixedLength;
    int lastPixelValue;
    int lastPressPoint;
    bool moving;
};

#endif // SLIDER_H
