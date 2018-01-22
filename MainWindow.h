#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "MenuButton.h"
#include "Content.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void menuButtonChecked(MenuButton*);
    void menuButtonUnchecked(MenuButton *);

protected:
    virtual void mousePressEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    Content *content;
};

#endif // MAINWINDOW_H
