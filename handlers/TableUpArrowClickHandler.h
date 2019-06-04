#ifndef TABLEUPARROWCLICKHANDLER_H
#define TABLEUPARROWCLICKHANDLER_H

#include <QObject>

class Table;

class TableUpArrowClickHandler : public QObject
{
    Q_OBJECT

public:
    TableUpArrowClickHandler(Table *table);
    void handle();

private:
    void checkLastRow();
    void checkPrevRow();

    /**
     * Возвращает -1 если такого ряда нет.
     */
    int findPrevVisibleRow(int fromRow) const;

    Table *table;
};

#endif // TABLEUPARROWCLICKHANDLER_H
