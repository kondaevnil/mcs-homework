#pragma once

#include "Figure.hpp"
#include <string>
#include <stdexcept>

class Circle : public Figure {
public:
    Circle(int id, int x, int y, int radius, const char* label);
    ~Circle() override;
    void print() const override;
    [[nodiscard]] bool is_inside(int x, int y) const override;
    void zoom(int factor) override;

private:
    std::string label_;
    int radius_;
};
