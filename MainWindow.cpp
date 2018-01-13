#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->setFixedWidth(240);

    QPalette shadowPalette;
    QPixmap shadowPixmap("images/shadow.png");
    shadowPalette.setBrush(QPalette::Background, QBrush(shadowPixmap));
    ui->widget_3->setPalette(shadowPalette);
    ui->widget_3->setFixedWidth(shadowPixmap.width());
}

MainWindow::~MainWindow()
{
    delete ui;
}
