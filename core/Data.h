#ifndef core_DATA_H
#define core_DATA_H

#include <QMap>
#include "core/Table.h"

namespace Core { class Data; };

class Core::Data
{
public:
    Data(const QString &file) noexcept;
    ~Data();

    void setFile(const QString &file) noexcept;

    QString getFile() const noexcept;
    const QMap<int, Table *> &getTables() const noexcept;

    /*void addTable(const Core::Table &table);
    void renameTable(int id, const QString &name);
    void removeTable(int id);*/

private:
    void loadTables() noexcept;
    void clearTables() noexcept;

    QString filename;
    QMap<int, Core::Table*> tables;
};

#endif // core_DATA_H