#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

// ==== PUBLIC ====
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->content->hide();

    QObject::connect(ui->menu, SIGNAL(buttonChecked(MenuButton*)), this, SLOT(menuButtonChecked(MenuButton*)));
    QObject::connect(ui->menu, SIGNAL(buttonUnchecked(MenuButton*)), this, SLOT(menuButtonUnchecked(MenuButton*)));
}
MainWindow::~MainWindow()
{
    delete ui;
}

// ==== PUBLIC SLOTS ====
void MainWindow::menuButtonChecked(MenuButton *)
{
    ui->content->show();
}
void MainWindow::menuButtonUnchecked(MenuButton *)
{
    ui->content->hide();
}

// ==== PROTECTED ====
void MainWindow::mousePressEvent(QMouseEvent *)
{
    if (!ui->content->isHidden()) ui->content->resetTableState();
}
