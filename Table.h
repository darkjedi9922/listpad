#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include "ui_Table.h"

namespace Ui {
class Table;
}

class Table : public QWidget
{
    Q_OBJECT

public:
    explicit Table(QWidget *parent = 0);
    ~Table();
    void addRow(QLabel *name, QLabel *status, QLabel *rating, QLabel *comment);
    void deleteRow(int row);
    void setRowChecked(int row, bool checked);
    int getCheckedRow() const;
    int getRowCount() const;

signals:
    void rowChecked();
    void rowsUnchecked();
    void rowDeleted(int row);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);

private:
    void updateMinHeight();
    int findRow(const QPoint &point) const;

    Ui::Table *ui;
    QBrush checkedRowBrush;
    int checkedRow;
    int rowHeight;
    int rowCount; // layout после удаления ряда все еще содержит его, а нужно знать количество рядов с элементами
};

#endif // TABLE_H
