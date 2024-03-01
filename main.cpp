#include "MainWindow.h"
#include "core/SqlData.h"
#include <QApplication>
#include <QSettings>
#include <QFontDatabase>
#include <QDir>

/**
 * Finds nearest directory path containing specified filename
 * @param filename filename to search
 * @returns absolute path of a directory or empty string if it is not found
 */
QString findNearestAncestorDirectoryOfFile(const QString &filename)
{
  QDir dir = QDir::current();
  while (!dir.isRoot() && !dir.exists(filename)) {
    dir.cdUp();
  }
  return dir.exists(filename) ? dir.absolutePath() : "";
}

/**
 * Application main function
 * @param argc count of command line arguments
 * @param argv command line arguments
 */
int main(int argc, char *argv[])
{
  auto projectRootPath = findNearestAncestorDirectoryOfFile("CMakeLists.txt");
  if (!projectRootPath.isEmpty()) {
    qputenv("QT_LOGGING_CONF", QByteArray::fromStdString(QDir(projectRootPath).filePath("logging.ini").toStdString()));
  }
  qSetMessagePattern("[%{time}] [%{type}] %{category}:%{line} - %{message}");

  QApplication a(argc, argv);
  QSettings settings("settings.ini", QSettings::IniFormat);
  Core::Data data("./data/main.xml");
  Core::SqlData db("./data/db.sqlite", data);
  MainWindow window;
  window.setSettings(&settings);
  window.setDatabase(&db);

  // В Linux по-умолчанию нет шрифта Arial. А, если есть, он почему-то может быть
  // не тем. Загрузим и будем использовать свой. Имя шрифта узнал через qDedbug().
  /* int id = */ QFontDatabase::addApplicationFont(":/fonts/arial.ttf"); // Arial.
  /* int id = */ QFontDatabase::addApplicationFont(":/fonts/arialbd.ttf");
  QFontDatabase::addApplicationFont(":/fonts/fontello.ttf");
  //qDebug() << QFontDatabase::applicationFontFamilies(id).at(0); // Имя.

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
