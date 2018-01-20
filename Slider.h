#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include "SliderItem.h"

class Slider : public QSlider
{
    Q_OBJECT

public:
    Slider(Qt::Orientation orientation, QWidget *parent = 0);
    void setValue(int value);
    void setSliderPosition(int position);
    void setSliderFixedLength(int length);
    void setSliderAutoLength();
    int getSliderLength();
    int getLength();

protected:
    virtual void paintEvent(QPaintEvent *);
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
    int sliderFixedLength;
    int lastPixelValue;
    int lastPressPoint;
    bool moving;
};

#endif // SLIDER_H
