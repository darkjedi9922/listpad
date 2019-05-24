#ifndef SEARCHLINE_H
#define SEARCHLINE_H

#include <QLineEdit>
#include <QPaintEvent>

namespace Ui {
class SearchLine;
}

class SearchLine : public QWidget
{
    Q_OBJECT

public:
    explicit SearchLine(QWidget *parent = nullptr);
    ~SearchLine();

    QLineEdit* getSearchLine() const;

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    Ui::SearchLine *ui;
};

#endif // SEARCHLINE_H
