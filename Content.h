#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>
#include "core/SqlData.h"
#include "engines/SearchEngine.h"
#include <QLoggingCategory>

namespace Ui
{
    class Content;
}

class Content : public QWidget
{
    Q_OBJECT

public:
    explicit Content(QWidget *parent = nullptr);
    ~Content();

    void setDatabase(Core::SqlData *db);

    void show();
    void hide();
    void loadTable(int id);
    void emptyTable();

    int getCurrentTableId() const;

public slots:
    void tableRowChecked(Table::row_id rowId);
    void tableRowsUnchecked();
    void tableRowDeleted();
    void tableRowEditingStarted(int row);
    void tableRowEditingFinished(Table::row_id rowId);
    void tableRowTextEdited(Table::row_id rowId);
    void tableRowStarToggled(Table::row_id rowId, bool starred);
    void resetTableState();

    void addButtonClicked();
    void deleteButtonClicked();
    void editButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    void addTableRow(Table::row_id id, const Core::TableRow &data);
    void addTableEmptyRow(Table::row_id id);
    void createDataDirectoryIfItDoesNotExist();
    void updateTableScrollingByRow(Table::row_id row);
    void adjustHorizontalScrollForCellCursor(LineEdit *cell);

    Ui::Content *ui;
    Core::SqlData *db;
    SearchEngine *eSearch;
    QLoggingCategory loggingCategory;
    int tableId;
};

#endif // CONTENT_H
