#include "TableUpArrowClickHandler.h"
#include "Table.h"

TableUpArrowClickHandler::TableUpArrowClickHandler(Table *table):
    table(table)
{}

void TableUpArrowClickHandler::handle()
{
    if (table->getCheckedRow() == -1) checkLastRow();
    else checkPrevRow();
}

void TableUpArrowClickHandler::checkLastRow()
{
    // 0 ряд - заголовок таблицы
    if (table->getVisibleRowCount() > 1) {
        int lastVisibleRow = findPrevVisibleRow(table->getRowCount() - 1);
        table->checkRealRow(lastVisibleRow);
    }
}

void TableUpArrowClickHandler::checkPrevRow()
{
    int checkedRow = table->getCheckedRow();
    int prevRow = findPrevVisibleRow(checkedRow - 1);
    if (prevRow == -1) return;
    table->checkRealRow(prevRow);
}

int TableUpArrowClickHandler::findPrevVisibleRow(int fromRow) const
{
    if (fromRow < 1) return -1;
    if (table->isRealRowVisible(fromRow)) return fromRow;
    return findPrevVisibleRow(fromRow - 1);
}
