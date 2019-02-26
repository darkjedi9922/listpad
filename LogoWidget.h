#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>

class LogoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogoWidget(QWidget *parent = nullptr);
    QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QPixmap icon;
};

#endif // ICONWIDGET_H
