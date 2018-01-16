#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "MenuButton.h"
#include "Table.h"

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
    void setupNewTable();
    void unsetupTable();

public slots:
    void menuButtonClicked();
    void menuBackgroundClicked();
    void tableRowChecked();
    void tableRowsUnchecked();
    void addButtonClicked();
    void deleteButtonClicked();
    void editButtonClicked();

private:
    MenuButton *checkedMenuButton;
    Ui::MainWindow *ui;
    Table *currentTable;
};

#endif // MAINWINDOW_H
