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
    void tableRowChecked(int row);
    void tableRowsUnchecked();
    void tableRowDeleted();
    void tableRowEditingStarted(int row);
    void tableRowEditingFinished(Table::row_id rowId);
    void tableRowTextEdited(int row);
    void resetTableState();

    void addButtonClicked();
    void deleteButtonClicked();
    void editButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    void addTableRow(Table::row_id id, const QList<QString> &list);
    void addTableEmptyRow(Table::row_id id);
    void createDataDirectoryIfItDoesNotExist();
    void updateTableScrollingByRow(int row);
    void adjustHorizontalScrollForCellCursor(LineEdit *cell);

    Ui::Content *ui;
    Core::SqlData *db;
    SearchEngine *eSearch;
    QLoggingCategory loggingCategory;
    int tableId;
};

#endif // CONTENT_H
