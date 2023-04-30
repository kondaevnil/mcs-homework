#include "Circle.hpp"
#include <string>

Circle::Circle(int id, int x, int y, int radius, const char *label) : Figure(id, x, y)
{
    if (label == nullptr)
        throw std::invalid_argument("Null string was given");

    label_ = std::string(label);
    radius_ = radius;
}

Circle::~Circle() noexcept = default;

void Circle::print() const
{
    printf("Circle %d: x = %d y = %d radius = %d label = %s\n", id_, x_, y_, radius_, label_.c_str());
}

bool Circle::is_inside(int x, int y) const
{
    return (x_ - x) * (x_ - x) + (y_ - y) * (y_ - y) <= radius_ * radius_;
}

void Circle::zoom(int factor)
{
    if (factor <= 0)
        throw std::invalid_argument("Factor should be greater than zero.");

    radius_ *= factor;
}
