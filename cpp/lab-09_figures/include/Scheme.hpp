#pragma once

#include "Figure.hpp"

class Scheme {
public:
    explicit Scheme(int capacity);
    ~Scheme();

    void push_back_figure(Figure* fg);
    void remove_figure(int id); // элементы смещаются влево

    void print_all_figures() const;
    void zoom_figure(int id, int factor);
    [[nodiscard]] Figure * is_inside_figure(int x, int y) const; // если внутри нескольких фигур, то возвращается любая из них
    void move(int id, int new_x, int new_y);

private:
    [[nodiscard]] int find_id(int id) const;

    Figure** figures_; // размер массива задается в конструкторе и больше не растет
    int capacity_;
    int size_;
};
