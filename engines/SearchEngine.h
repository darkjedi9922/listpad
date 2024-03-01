#ifndef engines_SEARCHENGINE_H
#define engines_SEARCHENGINE_H

#include <QLineEdit>
#include <QLoggingCategory>
#include "../widgets/CollectionTable.h"

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QLineEdit *searchLine, CollectionTable *table);

    void reset();

signals:
    void searchResultsChanged();

private:
    void initTextChangedHanler();
    void doShowEachTableRow(std::function<bool (const QStringList &columns)> check);

    QLineEdit *searchLine;
    CollectionTable *table;
    QLoggingCategory loggingCategory;
};

#endif // engines_SEARCHENGINE_H
