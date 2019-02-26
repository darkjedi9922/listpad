#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QWidget>
#include "ScrollBar.h"

class ScrollArea : public QWidget
{
    Q_OBJECT
public:
    explicit ScrollArea(QWidget *parent = nullptr);
    virtual QSize sizeHint() const;

    void update();
    void setWidget(QWidget *widget);

    ScrollBar* getVerticalScrollBar() const;

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    ScrollBar *vScrollBar;
    QWidget *widget;

private slots:
    void vSliderValueChanged(int value);
};

#endif // SCROLLAREA_H
