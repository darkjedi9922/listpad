#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "MenuButton.h"
#include "Content.h"
#include <QSettings>
#include "core/Data.h"

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

    void setData(Core::Data *);
    void setSettings(QSettings *);

    Core::Data* getData() const;
    QSettings* getSettings() const;

public slots:
    void menuButtonChecked(MenuButton*);
    void menuButtonUnchecked(MenuButton *);

protected:
    virtual void mousePressEvent(QMouseEvent *);

private:
    void saveSettings();

    Ui::MainWindow *ui;
    Core::Data *data;
    QSettings *settings;
};

#endif // MAINWINDOW_H
