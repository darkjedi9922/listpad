#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <QWidget>
#include "ui_Table.h"
#include <QLineEdit>
#include <QList>
#include <QLoggingCategory>
#include "widgets/elements/LineEdit.h"
#include "handlers/TableDownArrowClickHandler.h"
#include "handlers/TableUpArrowClickHandler.h"

namespace Ui {
class Table;
}
/*
 * Real Rows включают заголовок таблицы.
 */
class Table : public QWidget
{
    Q_OBJECT

public:
    /**
     * Unique ID of a row that is bound to the row initially and never changes
     */
    typedef unsigned long long row_id;

signals:
    void rowChecked(int row);
    void rowsUnchecked();
    void rowAdded(int row);
    void rowDeleted(int row);
    void editingStarted(int row);
    void editingFinished(int row);
    void editingFinishedById(Table::row_id rowId);
    void rowTextEdited(int row);
    void cellCursorPositionChanged(LineEdit *cell);

public:
    explicit Table(QWidget *parent = nullptr);
    ~Table();
    virtual QSize sizeHint() const;

    QList<QString> getRow(int row) const;
    QList<QString> getRow(row_id row) const;
    bool hasRow(int row) const;
    bool isStringsEmpty(int row) const;
    bool isStringsEmpty(row_id rowId) const;
    void setRowVisible(int row, bool);
    bool isRowVisible(int row) const;

    // adding
    void insertRowAfter(row_id id, const QList<QString> &list, int row);
    void appendRow(row_id, const QList<QString> &list);
    void deleteRow(row_id rowId);
    void deleteRow(int row);
    void empty();
    int getLastAddedRow() const;

    // checking
    void checkRow(int row);
    void uncheckRows();

    /**
     * Если никакой ряд не выделен, возвращает -1
     */
    int getCheckedRow() const;
    row_id getCheckedRowId() const;

    int getEditingRow() const;

    // geometry
    int getColumnCount() const;
    int getRowCount() const;
    int getVisibleRowCount() const;
    const QRect getRowRect(int row) const;

public slots:
    void startRowEditing(int row);
    void endRowsEditing();

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void keyPressEvent(QKeyEvent *);

private:
    void replaceRow(int from, int to);

    int toActualRow(int visibleRow) const;
    int toVisibleRow(int actualRow) const;
    int getRowById(row_id rowId) const;

    // finding
    int findRow(const QPoint &point) const;
    QLineEdit* getItemAt(int row, int column) const;

    Ui::Table *ui;
    QBrush checkedRowBrush;
    QLoggingCategory loggingCategory;

    /**
     * Если никакой ряд не выделен, равен -1.
     */
    int checkedRow;
    int deletingRow;
    int rowCount;
    int visibleRowCount;
    int editingRow;
    int lastAddedRow;
    std::map<row_id, int> rowsById;
    std::map<int, row_id> idsByRow;

    TableDownArrowClickHandler downArrowClickHandler;
    TableUpArrowClickHandler upArrowClickHandler;
};

#endif // TABLE_H
