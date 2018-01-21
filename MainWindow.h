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
    void blockMousePressed();
    void logoMousePressed();

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
    void tableRowAdded();
    void tableRowDeleted();
    void tableRowEdited();

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    void setupStartConnectings();

    // block
    void openBlock();
    void closeBlock();
    void updateBlockMaxHeight();
    void updateScrollAreaMinWidth();

    // table
    void setupNewTable();
    void removeOldTable();
    void updateTableSize();

    // table rows
    void addTableRow();
    void updateTableScrollingByRow(int row);

    Ui::MainWindow *ui;
    Table *currentTable;
};

#endif // MAINWINDOW_H
