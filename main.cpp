#include "MainWindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings settings("settings.ini", QSettings::IniFormat);
    Core::Data data("./data/main.xml");
    MainWindow window;
    window.setSettings(&settings);
    window.setData(&data);

    bool maximized = settings.value("maximized", false).toBool();
    if (maximized) {
        window.showMaximized();
    } else {
        QRect geometry = settings.value("geometry", QRect(100, 100, 1000, 600)).toRect();
        window.show();
        window.setGeometry(geometry);
    }

    return a.exec();
}