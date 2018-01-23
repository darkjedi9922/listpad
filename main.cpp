#include "MainWindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    bool maximized = window.getSettings()->value("maximized", false).toBool();
    if (maximized) {
        window.showMaximized();
    } else {
        QRect geometry = window.getSettings()->value("geometry", QRect(250, 250, 800, 400)).toRect();
        window.show();
        window.setGeometry(geometry);
    }

    return a.exec();
}
