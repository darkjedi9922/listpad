#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>
#include <QLineEdit>
#include <QEvent>

class MenuButton : public QPushButton
{
    Q_OBJECT
public:
    MenuButton(QWidget *parent = nullptr);
    ~MenuButton();

    void setMenuId(int id);
    int getMenuId() const;

public slots:
    void startEditing();
    void endEditing();

signals:
    void edited();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

private:
    QRect calcTextRect() const;

    int menuId;
    bool hovered;
    QBrush hoverBackgroundBrush;
    QBrush checkedBackgroundBrush;
    QColor textColor;
    QColor checkedTextColor;
    QFont textFont;
    QLineEdit *editingInput;
};

#endif // MENUBUTTON_H
