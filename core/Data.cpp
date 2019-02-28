#include "core/Data.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
using namespace Core;

Data::Data(const QString &file) noexcept
{
    this->setFile(file);
}

Data::~Data()
{
    clearTables();
}

void Data::setFile(const QString &file) noexcept
{
    this->filename = file;
    this->loadTables();
}

QString Data::getFile() const noexcept
{
    return filename;
}

const QMap<int, Table*>& Data::getTables() const noexcept
{
    return tables;
}

void Data::addTable(const Core::Table &table)
{
    if (tables.contains(table.getId()))
        throw "Table adding error: Table exists yet.";
    tables[table.getId()] = new Core::Table(table);
    saveTables();
}

void Data::loadTables() noexcept
{
    clearTables();
    if (!QFile::exists(filename)) return;
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QFile::Text);
    QXmlStreamReader reader(&file);
    do {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == "table") {
            int tableId = reader.attributes().value("id").toInt();
            QString tableName = reader.attributes().value("name").toString();
            QString tableFile = reader.attributes().value("file").toString();
            auto *table = new Core::Table(tableId, tableName, tableFile);
            tables.insert(tableId, table);
        }
    }
    while (!reader.atEnd());
}

void Data::saveTables() noexcept
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QFile::Text);

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("tables");

    foreach (Core::Table* table, tables) {
        writer.writeStartElement("table");
        writer.writeAttribute("id", QString("%1").arg(table->getId()));
        writer.writeAttribute("name", table->getName());
        writer.writeAttribute("file", table->getFilename());
        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

void Data::clearTables() noexcept
{
    foreach (Core::Table* table, tables) {
        delete table;
    }
    tables.clear();
}