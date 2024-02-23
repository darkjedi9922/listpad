#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "widgets/elements/MenuItem.h"
#include "Content.h"
#include <QSettings>
#include <QLoggingCategory>
#include "core/Data.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Загрузка всех категорий происходит здесь.
    void setData(Core::Data *);

    void setSettings(QSettings *);
    Core::Data* getData() const;
    QSettings* getSettings() const;

public slots:
    void menuButtonChecked(MenuItem *);
    void menuButtonUnchecked(MenuItem *);

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    void addCategoryToDatabase(int id, const QString &name);
    void saveSettings();

    QLoggingCategory loggingCategory;
    Ui::MainWindow *ui;
    Core::Data *data;
    QSettings *settings;
};

#endif // MAINWINDOW_H
