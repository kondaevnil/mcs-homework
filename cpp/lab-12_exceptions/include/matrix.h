#ifndef LAB_12_MATRIX_H
#define LAB_12_MATRIX_H

#include <vector>
#include <ostream>
#include <fstream>
#include <stdexcept>

class MatrixException : public std::runtime_error
{
public:
    explicit MatrixException(const std::string &s) : std::runtime_error(s) {}
};


class Matrix
{
public:
    explicit Matrix(const std::string &filename);
    Matrix();
    Matrix(const Matrix &m);
    Matrix(std::size_t rows, std::size_t cols);
    ~Matrix() = default;

    Matrix & operator=(Matrix &&m) noexcept;
    Matrix & operator+=(const Matrix &m);
    Matrix & operator*=(const Matrix &m);

    [[nodiscard]] int get(int i, int j) const;
    void print(std::ostream &os) const;

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<std::vector<int>> data_;
};

#endif