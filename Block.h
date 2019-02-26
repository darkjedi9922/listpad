#ifndef BLOCK_H
#define BLOCK_H

#include <QWidget>

class Block : public QWidget
{
    Q_OBJECT

public:
    explicit Block(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QBrush background;
    QPoint backgroundRadius;
};

#endif // BLOCK_H
