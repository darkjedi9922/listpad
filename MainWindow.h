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
    void resetTableFocus();

    // menu
    void newMenuButtonChecked(MenuButton*);
    void menuButtonUnchecked(MenuButton *);

    // table buttons
    void addButtonClicked();
    void deleteButtonClicked();
    void editButtonClicked();

    // table rows
    void tableRowChecked(int row);
    void tableRowsUnchecked();
    void tableRowDeleted();
    void tableRowEdited();

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    void setupStartConnectings();

    // block
    void updateBlockMaxHeight();
    void updateScrollAreaMinWidth();

    // table
    void setupTable();
    void removeTable();
    void updateTableSize();

    // table rows
    void addTableRow();
    void updateTableScrollingByRow(int row);

    Ui::MainWindow *ui;
    Table *currentTable;
};

#endif // MAINWINDOW_H
