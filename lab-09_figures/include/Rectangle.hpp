#include "Figure.hpp"

class Rectangle : public Figure {
public:
    Rectangle(int id, int x, int y, int width, int height);
    ~Rectangle() override;
    void print() const override;
    [[nodiscard]] bool is_inside(int x, int y) const override;
    void zoom(int factor) override;

private:
    int width_;
    int height_;
};
