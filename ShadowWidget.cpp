#include "ShadowWidget.h"

ShadowWidget::ShadowWidget(QWidget *parent) : QWidget(parent)
{
    // Фон
    setAutoFillBackground(true);
    QPalette shadowPalette;
    QPixmap shadowPixmap("images/shadow.png");
    shadowPalette.setBrush(QPalette::Background, QBrush(shadowPixmap));
    setPalette(shadowPalette);

    // Размер
    setFixedWidth(shadowPixmap.width());
}
