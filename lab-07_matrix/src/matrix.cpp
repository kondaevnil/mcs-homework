#include "matrix.h"
#include <stdexcept>
#include <cstdio>

Matrix::Matrix(size_t r, size_t c)
{
    if (r == 0 || c == 0)
        throw std::invalid_argument("Width and height must be greater than zero.");

    _cols = c;
    _rows = r;
    _data = new int*[_rows];

    for (int i = 0; i < _rows; i++)
    {
        _data[i] = new int[_cols];

        for (int j = 0; j < _cols; j++)
            _data[i][j] = 0;
    }
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

void Matrix::print(FILE *f) const
{
    if (f == nullptr)
        throw std::invalid_argument("Null pointer argument.");

    for (int i = 0; i < _rows; i++)
    {
        for (int j = 0; j < _cols; j++)
            fprintf(f, "%i%c", _data[i][j], j == _cols - 1 ? '\n' : ' ');
    }
}

Matrix::Matrix(const Matrix &m) : _cols(m._cols), _rows(m._rows)
{
    _data = new int*[_rows];

    for (int i = 0; i < _rows; i++)
    {
        _data[i] = new int[_cols];

        for (int j = 0; j < _cols; j++)
            _data[i][j] = m._data[i][j];
    }
}

Matrix::Matrix(Matrix &&m) noexcept
{
    _data = nullptr;
    _rows = 0;
    _cols = 0;

    std::swap(_data, m._data);
    std::swap(_rows, m._rows);
    std::swap(_cols, m._cols);
}

Matrix::~Matrix()
{
    for (int i = 0; i < _rows; i++)
        delete[] _data[i];

    delete[] _data;
}

Matrix & Matrix::operator=(const Matrix &m)
{
    if (this == &m)
        return *this;

    if (_rows != m._rows || _cols != m._cols)
    {
        for (int i = 0; i < _rows; i++)
            delete[] _data[i];

        delete[] _data;

        _rows = m._rows;
        _cols = m._cols;
        _data = new int*[_rows];

        for (int i = 0; i < _rows; i++)
            _data[i] = new int[_cols];
    }

    for (int i = 0; i < _rows; i++)
    {
        for (int j = 0; j < _cols; j++)
            _data[i][j] = m._data[i][j];
    }

    return *this;
}

Matrix & Matrix::operator=(Matrix &&m) noexcept
{
    if (*this != m)
    {
        std::swap(_data, m._data);
        std::swap(_rows, m._rows);
        std::swap(_cols, m._cols);
    }

    return *this;
}

bool Matrix::operator==(Matrix &m) const
{
    if (_rows != m._rows || _cols != m._cols)
        return false;

    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            if (_data[i][j] != m._data[i][j])
                return false;

    return true;
}

bool Matrix::operator!=(Matrix &m) const
{
    return !(*this == m);
}


Matrix Matrix::operator+(Matrix &m) const
{
    if (_rows != m._rows || _cols != m._cols)
        throw std::invalid_argument("Different matrix size.");

    Matrix tmp(*this);

    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            tmp._data[i][j] += m._data[i][j];

    return tmp;
}

Matrix Matrix::operator-(Matrix &m) const
{
    if (_rows != m._rows || _cols != m._cols)
        throw std::invalid_argument("Different matrix size.");

    Matrix tmp(*this);

    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            tmp._data[i][j] -= m._data[i][j];

    return tmp;
}

Matrix Matrix::operator*(Matrix &m) const
{
    if (_cols != m._rows)
        throw std::invalid_argument("Different matrix size.");

    Matrix tmp(_rows, m._cols);

    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < m._cols; j++)
            for (int k = 0; k < _cols; k++)
                tmp._data[i][j] += _data[i][k] * m._data[k][j];

    return tmp;
}

Matrix & Matrix::operator+=(Matrix &m)
{
    if (_rows != m._rows || _cols != m._cols)
        throw std::invalid_argument("Different matrix size.");

    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            _data[i][j] += m._data[i][j];
    return *this;
}

Matrix & Matrix::operator-=(Matrix &m)
{
    if (_rows != m._rows || _cols != m._cols)
        throw std::invalid_argument("Different matrix size.");

    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            _data[i][j] -= m._data[i][j];
    return *this;
}

Matrix & Matrix::operator*=(Matrix &m)
{
    if (_cols != m._rows)
        throw std::invalid_argument("Different matrix size.");

    Matrix tmp(_rows, m._cols);

    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < m._cols; j++)
            for (int k = 0; k < _cols; k++)
                tmp._data[i][j] += _data[i][k] * m._data[k][j];

    return *this = std::move(tmp);
}
