#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include "ui_Table.h"
#include <QLineEdit>
#include <QList>

namespace Ui {
class Table;
}

class Table : public QWidget
{
    Q_OBJECT

signals:
    void rowChecked();
    void rowsUnchecked();
    void rowDeleted(int row);

public:
    explicit Table(QWidget *parent = 0);
    ~Table();
    QSize sizeHint() const;

    void addRow(const QList<QString> &list);
    void deleteRow(int row);
    void setRowChecked(int row, bool checked);
    void startRowEditing(int row);
    int getCheckedRow() const;
    int getRowCount() const;
    int getRowHeight() const;

public slots:
    void endRowsEditing();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void keyPressEvent(QKeyEvent *);

private:
    void updateMinHeight();
    int findRow(const QPoint &point) const;
    QLineEdit* getItemAt(int row, int column) const;

    Ui::Table *ui;
    QBrush checkedRowBrush;
    int checkedRow;
    int rowHeight;
    int rowCount; // layout после удаления ряда все еще содержит его, а нужно знать количество рядов с элементами
    int editingRow;
};

#endif // TABLE_H
