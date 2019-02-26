#ifndef DESKTOPBUTTON_H
#define DESKTOPBUTTON_H

#include <QPushButton>

class DesktopButton : public QPushButton
{
public:
    DesktopButton(QWidget *parent = nullptr);
    QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

private:
    QPoint borderRadius;
    QFont textFont;
    QBrush normalBackground;
    QColor normalTextColor;
    QPixmap hoverBackgroundPartOne;
    QPixmap hoverBackgroundPartTwo;
    QPixmap hoverBackgroundPartThree;
    QColor hoverTextColor;
    bool hovered;
};

#endif // DESKTOPBUTTON_H
