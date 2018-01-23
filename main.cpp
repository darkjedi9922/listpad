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
        QRect geometry = window.getSettings()->value("geometry", QRect(100, 100, 1000, 600)).toRect();
        window.show();
        window.setGeometry(geometry);
    }

    return a.exec();
}
