#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QScrollBar>

class ScrollBar : public QScrollBar
{
public:
    ScrollBar(Qt::Orientation, QWidget *parent = 0);
    void setValue(int value);
    void setDocumentLength(int length);
    int value() const;
    int getDocumentLength() const;
    const QRect calculateSliderRect() const;

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    int documentLength;
    int currentValue;
    int lastMousePressPoint;
    int lastValue;
    bool moving;
};

#endif // SCROLLBAR_H
