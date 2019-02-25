#include "MainWindow.h"
#include <QApplication>
#include <QSettings>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings settings("settings.ini", QSettings::IniFormat);
    Core::Data data("./data/main.xml");
    MainWindow window;
    window.setSettings(&settings);
    window.setData(&data);

    // В Linux по-умолчанию нет шрифта Arial. А, если есть, он почему-то может быть
    // не тем. Загрузим и будем использовать свой. Имя шрифта узнал через qDedbug().
    // QString family = QFontDatabase::applicationFontFamilies(id).at(0); // Имя.
    /*int id =*/ QFontDatabase::addApplicationFont(":/fonts/arial.ttf"); // Arial.

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