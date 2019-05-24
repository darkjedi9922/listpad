#include "widgets/elements/SearchLine.h"
#include "ui_SearchLine.h"
#include <QPainter>

SearchLine::SearchLine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchLine)
{
    ui->setupUi(this);

    setFixedWidth(200);
    setFixedHeight(30);

    ui->searchLine->setStyleSheet(
        QString("QLineEdit {"
                "   background: transparent;"
                "   border: none;"
                "   font-size: 16px;"
                "   font-family: Arial;"
                "   color: white;"
                "}")
    );
}

SearchLine::~SearchLine()
{
    delete ui;
}

QLineEdit *SearchLine::getSearchLine() const
{
    return ui->searchLine;
}

void SearchLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#003366"));
    painter.drawRoundedRect(rect(), 10, 10);
}
