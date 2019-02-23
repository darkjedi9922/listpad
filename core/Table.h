#ifndef core_TABLE_H
#define core_TABLE_H

#include <QString>

namespace Core { class Table; }

class Core::Table
{
public:
    Table(int id, const QString &name, const QString &filename);

    void setId(int);
    void setName(const QString &);
    void setFilename(const QString &);

    int getId() const;
    QString getName() const;
    QString getFilename() const;

private:
    int id;
    QString name;
    QString filename;
};

#endif // core_TABLE_H