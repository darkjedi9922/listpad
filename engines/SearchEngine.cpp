#include "engines/SearchEngine.h"
#include <QStringList>

SearchEngine::SearchEngine(QLineEdit *searchLine, CollectionTable *table) :
    loggingCategory("SearchEngine")
{
    this->searchLine = searchLine;
    this->table = table;
    initTextChangedHanler();
}

void SearchEngine::reset()
{
    searchLine->setText("");
}

void SearchEngine::initTextChangedHanler()
{
    QObject::connect(searchLine, &QLineEdit::textChanged, [&] (const QString &searchPattern) {
        doShowEachTableRow([&] (const QString &title) {
            if (searchPattern == "") return true;
            return title.contains(searchPattern, Qt::CaseInsensitive);
        });
    });
}

void SearchEngine::doShowEachTableRow(std::function<bool (const QString &title)> check)
{
    int rowCount = table->getRowCount();
    for (int i = 1; i < rowCount; ++i) {
        bool doShow = check(table->getRow(i).title);
        table->setRowVisible(i, doShow);
    }
    emit searchResultsChanged();
}
