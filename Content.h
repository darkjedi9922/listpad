#ifndef CONTENT_H
#define CONTENT_H

#include "Block.h"
#include "core/Data.h"
#include "engines/SearchEngine.h"
#include <QLoggingCategory>

namespace Ui
{
    class Content;
}

class Content : public Block
{
    Q_OBJECT

public:
    explicit Content(QWidget *parent = nullptr);
    ~Content();

    void setData(Core::Data *data);

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

private:
    void addTableRow(const QList<QString> &list);
    void addTableEmptyRow();
    void createDataDirectoryIfItDoesNotExist();
    void updateTableScrollingByRow(int row);

    Ui::Content *ui;
    Core::Data *data;
    SearchEngine *eSearch;
    QLoggingCategory loggingCategory;
    int tableId;
};

#endif // CONTENT_H
