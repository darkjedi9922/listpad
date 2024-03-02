#include "SqlData.h"
#include "TableRows.h"
#include <QFileInfo>

using namespace Core;

SqlData::SqlData(const QString &filepath, const Data &initialData) :
  loggingCategory("SqlData")
{
  ensureDirExists(filepath);
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(filepath);
  if (!db.open()) {
    throw db.lastError().text();
  }
  enableForeignKeys();
  if (db.tables().size() == 0) {
    createTables();
    importData(initialData);
  }
}
SqlData::~SqlData()
{
  db.close();
}

QSqlQuery SqlData::getCategories()
{
  QSqlQuery query("SELECT * FROM tables");
  execDbQuery(query);
  return query;
}
QSqlQuery SqlData::getTableRows(int tableId)
{
  QSqlQuery query;
  query.prepare("SELECT * FROM rows WHERE table_id = :table_id");
  query.bindValue(":table_id", tableId);
  execDbQuery(query);
  return query;
}
void SqlData::addTable(int id, const QString &name)
{
  QSqlQuery query;
  query.prepare("INSERT INTO tables (id, name) VALUES (:id, :name)");
  query.bindValue(":id", id);
  query.bindValue(":name", name);
  execDbQuery(query);
}
void SqlData::renameTable(int id, const QString &name)
{
  QSqlQuery query;
  query.prepare("UPDATE tables SET name = :name WHERE id = :id");
  query.bindValue(":id", id);
  query.bindValue(":name", name);
  execDbQuery(query);
}
void SqlData::removeTable(int id)
{
  QSqlQuery query;
  query.prepare("DELETE FROM tables WHERE id = :id");
  query.bindValue(":id", id);
  execDbQuery(query);
}
size_t SqlData::addTableRow(int tableId, const Core::TableRow &row)
{
  QSqlQuery query;
  query.prepare(R"(
    INSERT INTO rows (table_id, title, status, rating, comment)
    VALUES (:table_id, :title, :status, :rating, :comment)
  )");
  query.bindValue(":table_id", tableId);
  query.bindValue(":title", row.title);
  query.bindValue(":status", row.status);
  query.bindValue(":rating", row.rating);
  query.bindValue(":comment", row.comment);
  execDbQuery(query);
  return query.lastInsertId().toULongLong();
}
void SqlData::updateTableRow(size_t rowId, const Core::TableRow &row)
{
  QSqlQuery query;
  query.prepare(R"(
    UPDATE rows SET
      title = :title,
      status = :status,
      rating = :rating,
      comment = :comment,
      starred = :starred
    WHERE id = :row_id
  )");
  query.bindValue(":row_id", rowId);
  query.bindValue(":title", row.title);
  query.bindValue(":status", row.status);
  query.bindValue(":rating", row.rating);
  query.bindValue(":comment", row.comment);
  query.bindValue(":starred", row.starred);
  execDbQuery(query);
}
void SqlData::updateTableRowStarred(size_t rowId, bool starred)
{
  QSqlQuery query;
  query.prepare("UPDATE rows SET starred = :starred WHERE id = :row_id");
  query.bindValue(":starred", starred);
  query.bindValue(":row_id", rowId);
  execDbQuery(query);
}
void SqlData::removeTableRow(size_t rowId)
{
  QSqlQuery query;
  query.prepare("DELETE FROM rows WHERE id = :row_id");
  query.bindValue(":row_id", rowId);
  execDbQuery(query);
}

void SqlData::createTables()
{
  execDbQuery("PRAGMA foreign_keys = ON");
  qCInfo(loggingCategory) << "Enabled foreign keys support";

  execDbQuery(R"(
    CREATE TABLE tables (
      id INTEGER NOT NULL PRIMARY KEY,
      name TEXT NOT NULL
    );
  )");
  execDbQuery(R"(
    CREATE TABLE rows (
      id INTEGER NOT NULL PRIMARY KEY,
      table_id INTEGER NOT NULL,
      starred INTEGER,
      title TEXT NOT NULL,
      status TEXT NOT NULL,
      rating TEXT NOT NULL,
      comment TEXT NOT NULL,
      FOREIGN KEY (table_id)
        REFERENCES tables (id) 
          ON UPDATE CASCADE
          ON DELETE CASCADE
    )
  )");
}
void SqlData::enableForeignKeys()
{
  execDbQuery("PRAGMA foreign_keys = ON");
  qCInfo(loggingCategory) << "Enabled foreign keys support";
  
  QSqlQuery query("PRAGMA foreign_keys");
  execDbQuery(query);
  query.next();
  qCInfo(loggingCategory) << "Foreign is enabled =" << query.value(0);
}
void SqlData::importData(const Core::Data &data)
{
  for (auto table : data.getTables()) {
    QSqlQuery query;
    query.prepare("INSERT INTO tables (id, name) VALUES (:id, :name)");
    query.bindValue(":id", table->getId());
    query.bindValue(":name", table->getName());
    execDbQuery(query);

    TableRows rows(table->getFilename());
    for (auto &row : rows.getRows()) {
      QSqlQuery query;
      query.prepare(R"(
        INSERT INTO rows (table_id, title, status, rating, comment)
        VALUES (:table_id, :title, :status, :rating, :comment)
      )");
      query.bindValue(":table_id", table->getId());
      query.bindValue(":title", row.at(0));
      query.bindValue(":status", row.at(1));
      query.bindValue(":rating", row.at(2));
      query.bindValue(":comment", row.at(3));
      execDbQuery(query);
    }
  }
}
QSqlQuery SqlData::execDbQuery(const QString &query)
{
  auto result = db.exec(query);
  if (db.lastError().isValid()) {
    auto error = QString("Query: " + query + "\nError: " + db.lastError().text());
    qCCritical(loggingCategory) << error;
    throw error;
  }
  return result;
}
void SqlData::execDbQuery(QSqlQuery &query)
{
  if (!query.exec()) {
    auto error = QString("Query: " + query.executedQuery() + "\nError: " + query.lastError().text());
    qCCritical(loggingCategory) << error;
    throw error;
  }
}
void SqlData::ensureDirExists(const QString &filepath)
{
  QFileInfo fileInfo(filepath);
  if (!fileInfo.absoluteDir().exists()) {
    ensureDirExists(fileInfo.absoluteDir().absolutePath());
    fileInfo.absoluteDir().mkdir(".");
  }
}
