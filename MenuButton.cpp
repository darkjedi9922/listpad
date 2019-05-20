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
    if (!editingInput) {
        editingInput = new QLineEdit(this);
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
    }

    editingInput->setText(this->text());
    editingInput->setGeometry(this->calcTextRect());

    this->setText("");

    editingInput->selectAll();
    editingInput->show();
    editingInput->setFocus();
    QObject::connect(editingInput, SIGNAL(editingFinished()), this, SLOT(endEditing()));
}

void MenuButton::endEditing()
{
    // Когда мы будем hide() input, он вызовет еще один сигнал editingFinished(),
    // который приведет к новому вызову данного слота. Чтобы не приводить к второму
    // вызову сигнала, отключаем объект от слотов совсем.
    editingInput->disconnect();

    setText(editingInput->text());
    editingInput->hide();
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
    auto iconMode = isChecked() ? QIcon::Selected : QIcon::Normal;
    // QIcon::paint() уместит иконку в QRect без растягивания картинки.
    icon().paint(&painter, 18, 0, height() - 18, height(), Qt::AlignCenter, iconMode);

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
    int spacingBetween = 9;
    return QRect(height() + spacingBetween, 0,
                 width() - height() - 18 - spacingBetween, height());
}
