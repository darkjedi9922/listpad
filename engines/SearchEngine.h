#ifndef engines_SEARCHENGINE_H
#define engines_SEARCHENGINE_H

#include <QLineEdit>
#include <QLoggingCategory>
#include "../widgets/CollectionTable.h"
#include "../widgets/elements/CheckBox.h"

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QLineEdit *searchLine, CheckBox *showStarred, CollectionTable *table);

    void reset();

signals:
    void searchResultsChanged();

private:
    void updateSearchResult();

    QLineEdit *searchLine;
    CheckBox *showStarred;
    CollectionTable *table;
    QLoggingCategory loggingCategory;
};

#endif // engines_SEARCHENGINE_H
