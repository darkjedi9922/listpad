#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include "ui_Table.h"
#include <QLineEdit>
#include <QList>

namespace Ui {
class Table;
}
/*
 * Real Rows включают заголовок таблицы.
 */
class Table : public QWidget
{
    Q_OBJECT

signals:
    void rowChecked(int row);
    void rowRechecked(int row);
    void rowsUnchecked();
    void rowAdded(int row);
    void rowDeleted(int row);
    void editingFinished(int row);

public:
    explicit Table(QWidget *parent = nullptr);
    ~Table();
    virtual QSize sizeHint() const;

    QList<QString> getRealRow(int row) const;
    bool hasRow(int row) const;
    bool isStringsEmpty(int row) const;
    void setRealRowVisible(int row, bool);
    bool isRealRowVisible(int row) const;

    // adding
    void insertRowAfter(const QList<QString> &list, int row);
    void appendRow(const QList<QString> &list);
    void deleteRow(int row);
    void empty();
    int getLastAddedRow() const;

    // checking
    void checkRealRow(int row);
    void uncheckRows();
    int getCheckedRow() const;

    int getEditingRow() const;

    // geometry
    int getColumnCount() const;
    int getRowCount() const;
    int getRowRealCount() const;
    int getRowHeight() const;
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
    bool checkRealRowWithoutEmit(int row);
    bool uncheckRowsWithoutEmit();

    int toActualRow(int visibleRow) const;
    int toVisibleRow(int actualRow) const;

    // finding
    int findRow(const QPoint &point) const;
    QLineEdit* getItemAt(int row, int column) const;

    Ui::Table *ui;
    QBrush checkedRowBrush;
    int checkedRealRow;
    int rowHeight;
    int rowCount;
    int visibleRealRowCount;
    int editingRow;
    int lastAddedRow;
};

#endif // TABLE_H
