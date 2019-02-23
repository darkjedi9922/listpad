#ifndef core_TABLEROWS_H
#define core_TABLEROWS_H

#include <QList>
#include <QString>

namespace Core { class TableRows; };

class Core::TableRows
{
public:
    TableRows(const QString &datafile);

    void setDatafile(const QString &);

    QString getDatafile() const;
    const QList<QList<QString>>& getRows() const;

private:
    void loadRows();

    QString datafile;
    QList<QList<QString>> rows;
};

#endif // core_TABLEROWS_H