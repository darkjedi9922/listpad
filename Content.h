#ifndef CONTENT_H
#define CONTENT_H

#include "Block.h"
#include "Table.h"

namespace Ui
{
    class Content;
}

class Content : public Block
{
    Q_OBJECT

public:
    explicit Content(QWidget *parent = 0);
    ~Content();
    virtual QSize sizeHint() const;

    void show();
    void hide();
    void loadTable(int id);
    void saveTable();
    void emptyTable();

    int getCurrentTableId() const;

public slots:
    void tableRowChecked(int row);
    void tableRowsUnchecked();
    void tableRowDeleted();
    void tableRowEdited(int row);
    void resetTableState();

    void addButtonClicked();
    void deleteButtonClicked();
    void editButtonClicked();

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    void addTableRow(const QList<QString> &list);
    void addTableEmptyRow();
    void updateTableSize();
    void updateTableScrollingByRow(int row);
    void updateScrollAreaMaxHeight();
    void updateScrollAreaMinWidth();

    Ui::Content *ui;
    Table *table;
    int tableId;
};

#endif // CONTENT_H
