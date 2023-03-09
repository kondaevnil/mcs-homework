#include <stdexcept>
#include "matrix.hpp"

Matrix::Matrix(std::size_t r, std::size_t c)
{
    if (r == 0 || c == 0)
        throw std::invalid_argument("Width and height must be greater than zero.");

    _cols = c;
    _rows = r;
    _data = new int*[_rows];

    for (std::size_t i = 0; i < _rows; i++)
    {
        _data[i] = new int[_cols];

        for (std::size_t j = 0; j < _cols; j++)
            _data[i][j] = 0;
    }

}

Matrix::~Matrix()
{
    for (std::size_t i = 0; i < _rows; i++)
        delete[] _data[i];

    delete[] _data;
}

size_t Matrix::get_rows() const
{
    return _rows;
}

size_t Matrix::get_cols() const
{
    return _cols;
}

void Matrix::set(size_t i, size_t j, int val)
{
    if (i >= _rows || j >= _cols)
        throw std::out_of_range("Some index is greater than matrix size.");

    _data[i][j] = val;
}

int Matrix::get(size_t i, size_t j) const
{
    if (i >= _rows || j >= _cols)
        throw std::out_of_range("Some index is greater than matrix size.");

    return _data[i][j];
}

bool Matrix::operator==(const Matrix &that) const
{
    if (_rows != that._rows || _cols != that._cols)
        return false;

    for (std::size_t i = 0; i < _rows; i++)
        for (std::size_t j = 0; j < _cols; j++)
            if (_data[i][j] != that._data[i][j])
                return false;

    return true;
}

bool Matrix::operator!=(const Matrix &that) const
{
    return !(*this == that);
}


