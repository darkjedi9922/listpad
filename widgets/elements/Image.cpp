#include "Image.h"

Image::Image(const QPixmap &image, const QSize &size, QWidget *parent) :
    Image::Image(parent)
{
    this->setImage(image, size);
}

Image::Image(QWidget *parent) : QWidget(parent)
{
    this->setAutoFillBackground(true);
}

QSize Image::sizeHint() const
{
    return getImage().size();
}

void Image::setImage(const QPixmap &image, const QSize &size)
{
    // Размеры
    QSize imageSize = size.isEmpty() ? image.size() : size;
    auto scaledImage = image.scaled(imageSize, Qt::IgnoreAspectRatio,
                                    Qt::SmoothTransformation);
    this->setFixedSize(imageSize);
    // Фон
    auto newPalette = this->palette();
    newPalette.setBrush(QPalette::Background, scaledImage);
    this->setPalette(newPalette);
}

QPixmap Image::getImage() const
{
    return this->palette().brush(QPalette::Background).texture();
}
