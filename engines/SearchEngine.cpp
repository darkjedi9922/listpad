#include "engines/SearchEngine.h"
#include <QStringList>

SearchEngine::SearchEngine(QLineEdit *searchLine, Table *table)
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
    auto handler = [&] (const QString &searchPattern) {
        doShowEachTableRow([&] (const QStringList &columns) {
            if (searchPattern == "") return true;
            auto name = columns[0];
            return name.contains(searchPattern, Qt::CaseInsensitive);
        });
    };

    QObject::connect(searchLine, &QLineEdit::textChanged, handler);
}

void SearchEngine::doShowEachTableRow(std::function<bool (const QStringList &columns)> check)
{
    int rowCount = table->getRowCount();
    for (int i = 1; i < rowCount; ++i) {
        if (!table->hasRow(i)) continue;
        auto columns = QStringList(table->getRow(i));
        bool doShow = check(columns);
        table->setRowVisible(i, doShow);
    }
    emit searchResultsChanged();
}
