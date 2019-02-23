#include "core/Table.h"
using namespace Core;

Table::Table(int id, const QString &name, const QString &filename)
{
    setId(id);
    setName(name);
    setFilename(filename);
}

void Table::setId(int id)
{
    this->id = id;
}

void Table::setName(const QString &name)
{
    this->name = name;
}

void Table::setFilename(const QString &filename)
{
    this->filename = filename;
}

int Table::getId() const
{
    return id;
}

QString Table::getName() const
{
    return name;
}

QString Table::getFilename() const
{
    return filename;
}