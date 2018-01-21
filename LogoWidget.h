#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>

class LogoWidget : public QWidget
{
    Q_OBJECT

signals:
    void mousePressed();

public:
    explicit LogoWidget(QWidget *parent = 0);
    QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);

private:
    QPixmap icon;
};

#endif // ICONWIDGET_H
