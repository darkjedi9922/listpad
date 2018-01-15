#include "Sides.h"
using namespace Ui;

template <class T>
Sides<T>::Sides() :
    left(T()),
    top(T()),
    right(T()),
    bottom(T())
{}

template <class T>
Sides<T>::Sides(const Ui::T &leftRight, const Ui::T &topBottom) :
    left(leftRight),
    right(leftRight),
    top(topBottom),
    bottom(topBottom)
{}

template <class T>
Sides<T>::Sides(const T &left, const T &top, const T &right, const T &bottom) :
    left(left),
    top(top),
    right(right),
    bottom(bottom)
{}

template <class T>
void Sides<T>::setLeft(const T &side)
{
    left = side;
}

template <class T>
void Sides<T>::setTop(const T &side)
{
    top = side;
}

template <class T>
void Sides<T>::setRight(const T &side)
{
    right = side;
}

template <class T>
void Sides<T>::setBottom(const T &side)
{
    bottom = side;
}

template <class T>
void Sides<T>::setSides(const T &leftRight, const T &topBottom)
{
    left = right = leftRight;
    top = bottom = topBottom;
}

template <class T>
void Sides<T>::setSides(const T &left, const T &top, const T &right, const T &bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
}

template <class T>
const T& Sides<T>::getLeft() const
{
    return left;
}

template <class T>
const T& Sides<T>::getTop() const
{
    return top;
}

template <class T>
const T& Sides<T>::getRight() const
{
    return right;
}

template <class T>
const T& Sides<T>::getBottom() const
{
    return bottom;
}