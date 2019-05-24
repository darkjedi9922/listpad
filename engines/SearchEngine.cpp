#include "engines/SearchEngine.h"
#include <QStringList>

SearchEngine::SearchEngine(QLineEdit *searchLine, Table *table)
{
    this->searchLine = searchLine;
    this->table = table;
    setupLineStyles();
    initTextChangedHanler();
}

void SearchEngine::reset()
{
    searchLine->setText("");
}

void SearchEngine::setupLineStyles()
{
    searchLine->setStyleSheet(
        QString("QLineEdit {"
                "   background: #003366;"
                "   border: none;"
                "   font-size: 16px;"
                "   font-family: Arial;"
                "   color: white;"
                "   padding: 4px 6px;"
                "   width: 200px"
                "}")
    );
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
    int rowCount = table->getRowRealCount();
    for (int i = 1; i < rowCount; ++i) {
        if (!table->hasRealRow(i)) continue;
        auto columns = QStringList(table->getRealRow(i));
        bool doShow = check(columns);
        table->setRealRowVisible(i, doShow);
    }
    emit searchResultsChanged();
}