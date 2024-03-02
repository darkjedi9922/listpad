#include "engines/SearchEngine.h"
#include <QStringList>

SearchEngine::SearchEngine(QLineEdit *searchLine, CheckBox* showStarred, CollectionTable *table) :
    loggingCategory("SearchEngine")
{
    this->searchLine = searchLine;
    this->showStarred = showStarred;
    this->table = table;
    QObject::connect(searchLine, &QLineEdit::textChanged, this, &SearchEngine::updateSearchResult);
    QObject::connect(showStarred, &CheckBox::toggled, this, &SearchEngine::updateSearchResult);
}

void SearchEngine::reset()
{
    searchLine->setText("");
}

void SearchEngine::updateSearchResult()
{
    auto searchPattern = searchLine->text();
    int rowCount = table->getRowCount();
    for (int i = 1; i < rowCount; ++i) {
        auto rowData = table->getRow(i);
        bool doShow = (searchPattern == "" || rowData.title.contains(searchPattern, Qt::CaseInsensitive)) &&
            (!showStarred->isChecked() || rowData.starred);
        table->setRowVisible(i, doShow);
    }
    emit searchResultsChanged();
}
