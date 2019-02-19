#ifndef LAYOUTITERATOR_H
#define LAYOUTITERATOR_H

#include <QBoxLayout>

/**
 * Использует static_cast при получении виджета (именно если виджет). Такой cast не
 * проверяет на ошибки, поэтому нужно быть увереным, что все виджеты в компоновщике
 * именно того типа, который используется в классе.
 *
 * Использование:
 * LayoutIterator<QWidget> it(pLayout);
 * while (auto item = it.next()) { item->... }
 *
 * На будущее: Все таки как ни пытайся, невозможно нормально поместить реализацию
 * шаблонного класса в отдельный .cpp файл.
 */
template <typename ItemType>
class LayoutIterator
{
public:
    LayoutIterator(QBoxLayout *layout)
    {
        this->layout = layout;
        this->currentIndex = 0;
    }

    ItemType* next()
    {
        if (currentIndex >= layout->count()) return nullptr;
        QLayoutItem* item = layout->itemAt(currentIndex++);
        if (!item || !item->widget()) return this->next();
        return static_cast<ItemType*>(item->widget());
    }

private:
    QBoxLayout *layout;
    int currentIndex;
};

#endif // LAYOUTITERATOR_H