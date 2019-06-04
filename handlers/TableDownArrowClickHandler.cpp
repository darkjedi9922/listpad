#include "TableDownArrowClickHandler.h"
#include "Table.h"

TableDownArrowClickHandler::TableDownArrowClickHandler(Table *table):
    table(table)
{}

void TableDownArrowClickHandler::handle()
{
    if (table->getCheckedRow() == -1) checkFirstRow();
    else checkNextRow();
}

void TableDownArrowClickHandler::checkFirstRow()
{
    // 0 ряд - заголовок таблицы
    if (table->getVisibleRowCount() > 1) {
        int firstVisibleRow = findNextVisibleRow(1);
        table->checkRealRow(firstVisibleRow);
    }
}

void TableDownArrowClickHandler::checkNextRow()
{
    int checkedRow = table->getCheckedRow();
    int nextRow = findNextVisibleRow(checkedRow + 1);
    if (nextRow == -1) return;
    table->checkRealRow(nextRow);
}

int TableDownArrowClickHandler::findNextVisibleRow(int fromRow) const
{
    if (fromRow >= table->getRowCount()) return -1;
    if (table->isRealRowVisible(fromRow)) return fromRow;
    return findNextVisibleRow(fromRow + 1);
}
