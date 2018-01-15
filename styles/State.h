#ifndef STATE_H
#define STATE_H

#include <QObject>

namespace Ui
{
    class State : public QObject
    {
        Q_OBJECT

    public:
        State(QObject *parent = 0);
        virtual ~State();

        bool isNormal() const;
        void setNormal();
        bool isHover() const;
        void setHover(bool);
        bool isChecked() const;
        void setChecked(bool);

    signals:
        void changed();

    private:
        bool hover;
        bool checked;
    };
}

#endif // STATE_H
