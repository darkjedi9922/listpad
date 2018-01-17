#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QScrollArea>

class ScrollArea : public QScrollArea
{
public:
    ScrollArea(QWidget *parent = 0);
    virtual QSize sizeHint() const;
};

#endif // SCROLLAREA_H
