#include "Scheme.hpp"
#include <stdexcept>
#include <algorithm>

Scheme::Scheme(int capacity)
{
    if (capacity <= 0)
        throw std::invalid_argument("Capacity value should be greater then zero.");

    capacity_ = capacity;
    size_ = 0;
    figures_ = new Figure * [capacity_];
}

Scheme::~Scheme()
{
    for (int i = 0; i < size_; i++)
        delete figures_[i];

    delete[] figures_;
}

void Scheme::push_back_figure(Figure *fg)
{
    if (size_ == capacity_)
        throw std::length_error("Maximum size was reached.");

    if (fg == nullptr)
        throw std::invalid_argument("Null pointer to figure.");

    figures_[size_++] = fg;
}

int Scheme::find_id(int id) const
{
    int index = 0;

    while (index < size_ && figures_[index]->get_id() != id)
        index++;

    return index;
}

void Scheme::remove_figure(int id)
{
    int index = find_id(id);

    if (index == size_)
        throw std::runtime_error("Figure with this id was not found.");

    delete figures_[index];

    if (index + 1 == size_)
    {
        size_--;
        return;
    }

    std::copy(figures_ + index + 1, figures_ + size_, figures_ + index);
    size_--;
}

void Scheme::print_all_figures() const
{
    for (int i = 0; i < size_; i++)
        figures_[i]->print();
}

void Scheme::zoom_figure(int id, int factor)
{
    int index = find_id(id);

    if (index == size_)
        throw std::runtime_error("Figure with this id was not found.");

    figures_[index]->zoom(factor);
}

Figure * Scheme::is_inside_figure(int x, int y) const
{
    for (int i = 0; i < size_; i++)
        if (figures_[i]->is_inside(x, y))
            return figures_[i];

    return nullptr;
}

void Scheme::move(int id, int new_x, int new_y)
{
    int index = find_id(id);

    if (index == size_)
        throw std::runtime_error("Figure with this id was not found.");

    figures_[index]->move(new_x, new_y);
}
