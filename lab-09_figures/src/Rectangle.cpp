#include "Rectangle.hpp"
#include <stdexcept>
#include <cstdio>

Rectangle::Rectangle(int id, int x, int y, int width, int height) : Figure(id, x, y)
{
    height_ = height;
    width_ = width;
}

bool Rectangle::is_inside(int x, int y) const
{
    return abs(x - x_) <= width_ / 2 && abs(y - y_) <= height_ / 2;
}

void Rectangle::zoom(int factor)
{
    height_ *= factor;
    width_ *= factor;
}

void Rectangle::print() const
{
    printf("Rectangle %d: x = %d y = %d width = %d height = %d\n", id_, x_, y_, width_, height_);
}

Rectangle::~Rectangle() = default;
