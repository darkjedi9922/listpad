#ifndef MENUITEM_H
#define MENUITEM_H

#include <QWidget>
#include <QLoggingCategory>

namespace Ui {
  class MenuItem;
}

class MenuItem : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedStateChanged)
  Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)
  Q_PROPERTY(QString text READ getText WRITE setText)
  Q_PROPERTY(QString checkedIcon READ getOverridenCheckedIconPath WRITE setCheckedIconPath)
  Q_PROPERTY(QString icon READ getOverridenIconPath WRITE setIconPath)

public:
  explicit MenuItem(QWidget *parent = nullptr);
  ~MenuItem();

  void setCheckable(bool checkable);
  bool isCheckable() const;
  void setChecked(bool checked);
  bool isChecked() const;
  void setMenuId(int id);
  int getMenuId() const;
  void setText(const QString& text);
  QString getText() const;
  void setIconPath(const QString& icon);
  QString getOverridenIconPath() const;
  void setCheckedIconPath(const QString &icon);
  QString getOverridenCheckedIconPath() const;

public slots:
  void startEditing();
  void endEditing();

signals:
  void checkedStateChanged(bool checked);
  void clicked();
  void edited();

protected:
  virtual void paintEvent(QPaintEvent *);
  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);

private:
  void updateStyleSheet();

  QLoggingCategory loggingCategory;
  QString overridenCheckedIconPath;
  QString overridenIconPath;
  QString originalQss;
  Ui::MenuItem *ui;
  bool checkable;
  bool checked;
  int menuId;
};

#endif // MENUITEM_H
