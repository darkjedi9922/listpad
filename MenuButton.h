#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>

class MenuButton : public QPushButton
{
    Q_OBJECT
public:
    MenuButton(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

private:
    bool hovered;
    QBrush hoverBackgroundBrush;
    QBrush checkedBackgroundBrush;
    QColor textColor;
    QColor checkedTextColor;
    QFont textFont;
    QPixmap normalIcon;
    QPixmap checkedIcon;
};

#endif // MENUBUTTON_H
