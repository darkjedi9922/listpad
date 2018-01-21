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
 * Этот класс создан на основе GridLayout. Но так как в нем
 * после удаления строк сами строки еще остаются и учитываются
 * при подсчете rowCount, в этом классе это нужно учитывать.
 */
class Table : public QWidget
{
    Q_OBJECT

signals:
    void rowChecked(int row);
    void rowsUnchecked();
    void rowAdded(int row);
    void rowDeleted(int row);
    void editingFinished();

public:
    explicit Table(QWidget *parent = 0);
    ~Table();
    virtual QSize sizeHint() const;

    // adding
    void insertRowAfter(const QList<QString> &list, int row);
    void appendRow(const QList<QString> &list);
    void deleteRow(int row);

    // checking
    void checkRow(int row);
    void uncheckRows();
    int getCheckedRow() const;

    int getEditingRow() const;

    // geometry
    int getRowCount() const;
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

    // finding
    bool hasRow(int row) const;
    int findRowBefore(int row) const;
    int findRowAfter(int row) const;
    int findRow(const QPoint &point) const;
    QLineEdit* getItemAt(int row, int column) const;

    Ui::Table *ui;
    QBrush checkedRowBrush;
    int checkedRow;
    int rowHeight;
    int rowCount;
    int editingRow;
};

#endif // TABLE_H
