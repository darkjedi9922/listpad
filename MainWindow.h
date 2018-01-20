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

public slots:
    // menu
    void newMenuButtonChecked(MenuButton*);
    void menuButtonUnchecked(MenuButton *);

    // table buttons
    void addButtonClicked();
    void deleteButtonClicked();
    void editButtonClicked();

    // table rows
    void tableRowChecked();
    void tableRowsUnchecked();

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    void setupStartConnectings();

    // block
    void openBlock();
    void closeBlock();
    void updateBlockMaxHeight();

    // table
    void setupNewTable();
    void removeOldTable();
    void hideTable();
    void showTable();
    void updateTableSize();

    // table rows
    void addTableRow();

    Ui::MainWindow *ui;
    Table *currentTable;
};

#endif // MAINWINDOW_H
