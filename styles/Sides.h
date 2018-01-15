#ifndef SIDES
#define SIDES

namespace Ui
{
    template <class T>
    class Sides
    {
    public:
        Sides();
        Sides(const T &leftRight, const T &topBottom);
        Sides(const T &left, const T &top, const T &right, const T &bottom);

        void setLeft(const T &side);
        void setTop(const T &side);
        void setRight(const T &side);
        void setBottom(const T &side);
        void setSides(const T &leftRight, const T &topBottom);
        void setSides(const T &left, const T &top, const T &right, const T &bottom);

        const T& getLeft() const;
        const T& getTop() const;
        const T& getRight() const;
        const T& getBottom() const;

    private:
        T left;
        T top;
        T right;
        T bottom;
    };
}

#endif // SIDES