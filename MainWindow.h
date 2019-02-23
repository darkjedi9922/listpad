#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "MenuButton.h"
#include "Content.h"
#include <QSettings>

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

    void setSettings(QSettings *);

    QSettings* getSettings() const;

public slots:
    void menuButtonChecked(MenuButton*);
    void menuButtonUnchecked(MenuButton *);

protected:
    virtual void mousePressEvent(QMouseEvent *);

private:
    void saveSettings();

    Ui::MainWindow *ui;
    QSettings *settings;
};

#endif // MAINWINDOW_H
