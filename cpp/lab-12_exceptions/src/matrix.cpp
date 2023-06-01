#include "matrix.h"

Matrix::Matrix()
{
    rows_ = 0;
    cols_ = 0;
}


Matrix::Matrix(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        throw MatrixException("LOAD: unable to open file.");

    file >> rows_;
    if (file.fail() || file.bad())
        throw MatrixException("LOAD: invalid file format.");

    file >> cols_;
    if (file.fail() || file.bad())
        throw MatrixException("LOAD: invalid file format.");

    data_ = std::vector<std::vector<int>>(rows_, std::vector<int>(cols_));

    for (int i = 0; i < rows_; i++)
    {
        for (int x, j = 0; j < cols_; j++)
        {
            file >> x;
            if (file.fail() || file.bad())
                throw MatrixException("LOAD: invalid file format.");

            data_[i][j] = x;
        }
    }
}

Matrix::Matrix(const Matrix &m)
{
    cols_ = m.cols_;
    rows_ = m.rows_;
    data_ = std::vector<std::vector<int>>(rows_, std::vector<int>(cols_));
}

Matrix::Matrix(std::size_t rows, std::size_t cols) : rows_(rows), cols_(cols)
{
    data_ = std::vector<std::vector<int>>(rows_, std::vector<int>(cols_, 0));
}


void Matrix::print(std::ostream &os) const
{
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            os << data_[i][j] << (j == cols_ - 1 ? "\n" : " ");
        }
    }
}


Matrix & Matrix::operator+=(const Matrix &m)
{
    if (rows_ != m.rows_ || cols_ != m.cols_)
        throw MatrixException("ADD: dimensions do not match.");

    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            data_[i][j] += m.data_[i][j];
        }
    }

    return *this;
}

Matrix & Matrix::operator*=(const Matrix &m)
{
    if (cols_ != m.rows_)
        throw MatrixException("MUL: #arg1.columns != #arg2.rows.");

    Matrix tmp(rows_, m.cols_);

    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < m.cols_; j++)
        {
            for (int k = 0; k < cols_; k++)
                tmp.data_[i][j] += data_[i][k] * m.data_[k][j];
        }
    }

    return *this = std::move(tmp);
}

int Matrix::get(int i, int j) const
{
    if (i < 0 || i >= rows_ || j < 0 || j >= cols_)
        throw MatrixException("ACCESS: bad index.");

    return data_[i][j];
}


Matrix & Matrix::operator=(Matrix &&m) noexcept
{
    std::swap(data_, m.data_);
    std::swap(rows_, m.rows_);
    std::swap(cols_, m.cols_);

    return *this;
}
