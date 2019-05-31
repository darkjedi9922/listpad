#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>

class Image : public QWidget
{
    Q_OBJECT
public:
    explicit Image(const QPixmap &, const QSize &size = QSize(),
                   QWidget *parent = nullptr);
    explicit Image(QWidget *parent = nullptr);
    virtual QSize sizeHint() const;

    /**
     * По умолчанию берется размер самой картинки.
     */
    void setImage(const QPixmap &, const QSize &size = QSize());
    QPixmap getImage() const;
};

#endif // IMAGE_H
