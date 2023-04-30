#pragma once

#include <cstddef>

class Matrix {
public:
    Matrix(std::size_t r, std::size_t c);
    ~Matrix();

    [[nodiscard]] std::size_t get_rows() const;
    [[nodiscard]] std::size_t get_cols() const;
    void set(std::size_t i, std::size_t j, int val);
    [[nodiscard]] int get(std::size_t i, std::size_t j) const;

    bool operator==(const Matrix& that) const;
    bool operator!=(const Matrix& that) const;

private:
    Matrix(const Matrix& that);
    Matrix& operator=(const Matrix& that);

private:
    std::size_t _rows;
    std::size_t _cols;
    int **_data;
};
