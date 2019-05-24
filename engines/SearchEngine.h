#ifndef engines_SEARCHENGINE_H
#define engines_SEARCHENGINE_H

#include <QLineEdit>
#include "Table.h"

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QLineEdit *searchLine, Table *table);

signals:
    void searchResultsChanged();

private:
    void initTextChangedHanler();
    void doShowEachTableRow(std::function<bool (const QStringList &columns)> check);

    QLineEdit *searchLine;
    Table *table;
};

#endif // engines_SEARCHENGINE_H
