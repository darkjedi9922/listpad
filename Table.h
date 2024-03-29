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
    void rowIdChecked(row_id rowId);
    void rowsUnchecked();
    void rowAdded(int row);
    void rowDeleted(int row);
    void rowHovered(row_id rowId);
    void rowUnhovered(row_id rowId);

public:
    explicit Table(QWidget *parent = nullptr);
    ~Table();
    virtual QSize sizeHint() const;

    QList<QWidget*> getRow(int row) const;
    QList<QWidget*> getRow(row_id rowId) const;
    QWidget* getCell(row_id rowId, int column) const;
    bool hasRow(int row) const;
    void setRowVisible(int row, bool);
    bool isRowVisible(int row) const;

    // adding
    void insertRowAfter(row_id id, const QList<QWidget*> &list, int row);
    void appendRow(row_id, const QList<QWidget*> &list);
    void deleteRow(row_id rowId);
    void deleteRow(int row);
    void empty();
    int getLastAddedRow() const;

    // checking
    void checkRow(int row);
    void checkRow(row_id rowId);
    void uncheckRows();

    /**
     * Если никакой ряд не выделен, возвращает -1
     */
    int getCheckedRow() const;
    row_id getCheckedRowId() const;

    // geometry
    int getColumnCount() const;
    int getRowCount() const;
    int getVisibleRowCount() const;
    const QRect getRowRect(int row) const;
    const QRect getRowRect(Table::row_id rowId) const;

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual bool event(QEvent *event);

private:
    void replaceRow(int from, int to);
    
    void onHoverMove(QHoverEvent *event);
    void onHoverLeave();

    int toActualRow(int visibleRow) const;
    int toVisibleRow(int actualRow) const;
    int getRowById(row_id rowId) const;

    // finding
    int findRow(const QPoint &point) const;
    QWidget* getItemAt(int row, int column) const;

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
    int lastAddedRow;
    int hoveredRow;
    std::map<row_id, int> rowsById;
    std::map<int, row_id> idsByRow;

    TableDownArrowClickHandler downArrowClickHandler;
    TableUpArrowClickHandler upArrowClickHandler;
};

#endif // TABLE_H
