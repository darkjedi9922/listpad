#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>

class MenuButton : public QPushButton
{
    Q_OBJECT
public:
    MenuButton(QWidget *parent = nullptr);
    void setMenuId(int id);
    int getMenuId() const;

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

private:
    int menuId;
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
