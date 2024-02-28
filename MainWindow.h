#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "widgets/elements/MenuItem.h"
#include "Content.h"
#include <QSettings>
#include <QLoggingCategory>
#include "core/SqlData.h"

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
    void setDatabase(Core::SqlData *);

    void setSettings(QSettings *);
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
    Core::SqlData *db;
    QSettings *settings;
};

#endif // MAINWINDOW_H
