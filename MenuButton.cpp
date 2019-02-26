#include "MenuButton.h"
#include <QPainter>

// ==== PUBLIC ====
MenuButton::MenuButton(QWidget *parent) : QPushButton(parent),
    menuId(0),
    hovered(false),
    hoverBackgroundBrush(QBrush(QColor("#383838"))),
    checkedBackgroundBrush(QColor("#0b0b0b")),
    textColor(QColor("#c5c5c5")),
    checkedTextColor(QColor("#979797")),
    textFont(QFont("Arial", 12, 200)),
    normalIcon(QPixmap(":images/folder.png")),
    checkedIcon(QPixmap(":images/checkedFolder.png")),
    editingInput(nullptr)
{
    setFixedHeight(36);
    setCheckable(true);
}

MenuButton::~MenuButton()
{
    if (editingInput) {
        delete editingInput;
        editingInput = nullptr;
    };
}

void MenuButton::setMenuId(int id)
{
    menuId = id;
}

int MenuButton::getMenuId() const
{
    return menuId;
}

void MenuButton::startEditing()
{
    editingInput = new QLineEdit(this->text(), this);
    this->setText("");
    editingInput->setGeometry(this->calcTextRect());
    editingInput->setStyleSheet(
        QString("QLineEdit {"
                "   border: 0px;"
                "   background: %1;"
                "   color: %2;"
                "   font-family: %3;"
                "   font-size: %4pt;"
                // textFont.weight() выдает 72. При том, что оно установлено в 200...
                // (200 - должно быть bold).
                "   font-weight: bold"
                "}")
                .arg("transparent")
                .arg(textColor.name())
                .arg(textFont.family())
                .arg(textFont.pointSize())
    );

    editingInput->selectAll();
    editingInput->show();
    editingInput->setFocus();
    QObject::connect(editingInput, SIGNAL(editingFinished()), this, SLOT(endEditing()));
}

void MenuButton::endEditing()
{
    // Когда мы будем hide() input, он вызовет еще один сигнал editingFinished(),
    // который приведет к новому вызову данного слота. В нем он delete, а потом,
    // когда выполнение функции вернется на уровень обратно, данный метод снова
    // попытается delete, а delete уже нечего. Чтобы не приводить к второму вызову
    // сигнала, отключаем объект от слотов совсем.
    editingInput->disconnect();

    setText(editingInput->text());
    editingInput->hide();
    delete editingInput;
    editingInput = nullptr;
    emit edited();
}

// ==== EVENTS ====
void MenuButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    // Фон
    if (isChecked()) painter.setBrush(checkedBackgroundBrush);
    else if (hovered) painter.setBrush(hoverBackgroundBrush);
    painter.drawRect(rect());

    // Иконка
    painter.drawPixmap(18, rect().center().y() - normalIcon.height() / 2,
                       normalIcon.width(), normalIcon.height(),
                       (isChecked() ? checkedIcon : normalIcon));

    // Текст
    if (isChecked()) painter.setPen(checkedTextColor);
    else painter.setPen(textColor);
    painter.setFont(textFont);
    painter.drawText(calcTextRect(), Qt::AlignVCenter, text());
}
void MenuButton::enterEvent(QEvent *)
{
    hovered = true;
    repaint();
}
void MenuButton::leaveEvent(QEvent *)
{
    hovered = false;
    repaint();
}

QRect MenuButton::calcTextRect() const
{
    return QRect(18 + normalIcon.width() + 9, 0,
                 width() - 18 - normalIcon.width() - 9, height());
}