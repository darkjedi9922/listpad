#ifndef TABLEDOWNARROWCLICKHANDLER_H
#define TABLEDOWNARROWCLICKHANDLER_H

#include <QObject>

class Table;

class TableDownArrowClickHandler : public QObject
{
    Q_OBJECT

public:
    TableDownArrowClickHandler(Table *table);
    void handle();

private:
    void checkFirstRow();
    void checkNextRow();

    /**
     * Возвращает -1 если такого ряда нет.
     */
    int findNextVisibleRow(int fromRow) const;

    Table *table;
};

#endif // TABLEDOWNARROWCLICKHANDLER_H
