#include "core/TableRows.h"
#include <QXmlStreamReader>
using namespace Core;

TableRows::TableRows(const QString &datafile)
{
    setDatafile(datafile);
}

void TableRows::setDatafile(const QString &datafile)
{
    this->datafile = datafile;
    loadRows();
}

QString TableRows::getDatafile() const
{
    return this->datafile;
}

const QList<QList<QString>>& TableRows::getRows() const
{
    return rows;
}

void TableRows::loadRows()
{
    rows.clear();
    if (!QFile::exists(datafile)) return;
    QFile file(datafile);
    file.open(QIODevice::ReadWrite | QFile::Text);
    QXmlStreamReader reader(&file);
    do {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == "row")
        {
            QList<QString> newRow;
            for (auto attr : reader.attributes()) {
                newRow.append(attr.value().toString());
            }
            rows.append(newRow);
        }
    }
    while (!reader.atEnd());
}