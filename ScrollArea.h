#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QWidget>
#include <QLoggingCategory>
#include "ScrollBar.h"

class ScrollArea : public QWidget
{
    Q_OBJECT
public:
    explicit ScrollArea(QWidget *parent = nullptr);
    ~ScrollArea();
    virtual QSize sizeHint() const;

    void update();
    void setWidget(QWidget *widget);

    ScrollBar* getVerticalScrollBar() const;

protected:
    virtual void resizeEvent(QResizeEvent *);
    bool event(QEvent *event) override;

private:
    QLoggingCategory loggingCategory;
    ScrollBar *vScrollBar;
    QWidget *widget;

private slots:
    void vSliderValueChanged(int value);
};

#endif // SCROLLAREA_H
