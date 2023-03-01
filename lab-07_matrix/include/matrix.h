#ifndef LAB_08_MATRIX_H
#define LAB_08_MATRIX_H

#include <cstdio>
#include <cstddef>

class Matrix {
public:
  Matrix(size_t r, size_t c);

  size_t get_rows();
  size_t get_cols();
  void set(size_t i, size_t j, int val);
  int get(size_t i, size_t j);
  void print(FILE *f);

  Matrix operator+(Matrix& m);
  Matrix operator-(Matrix& m);
  Matrix operator*(Matrix& m);

  Matrix& operator+=(Matrix& m);
  Matrix& operator-=(Matrix& m);
  Matrix& operator*=(Matrix& m);

  bool operator==(Matrix& m);
  bool operator!=(Matrix& m);
private:
  size_t _rows;
  size_t _cols;
  int **_data;
};

#endif
