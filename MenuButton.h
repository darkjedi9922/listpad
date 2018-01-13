#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>

class MenuButton : public QPushButton
{
public:
    MenuButton(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

private:
    bool hover;
    QBrush hoverBackgroundBrush;
    QColor textColor;
    QFont textFont;
    QPixmap normalIcon;
};

#endif // MENUBUTTON_H
