#include "my_vector.hpp"
#include <cstring>
#include <algorithm>

MyVector::MyVector()
{
    _data = new int[2];
    _capacity = 2;
    _size = 0;
}

MyVector::MyVector(std::size_t init_capacity)
{
    _data = new int[std::max(init_capacity, (std::size_t)2)];
    _capacity = init_capacity;
    _size = 0;
}

MyVector::MyVector(MyVector const &vector)
{
    _data = new int[vector._capacity];
    _capacity = vector._capacity;
    _size = vector._size;
    std::copy(vector._data, vector._data + vector._size, _data);
}

MyVector &MyVector::operator=(MyVector const &vector)
{
    if (this != &vector)
    {
        if (_capacity != vector._capacity)
        {
            delete[] _data;
            _capacity = vector._capacity;
            _data = new int[vector._capacity];
            _size = vector._size;
        }

        std::copy(vector._data, vector._data + vector._size, _data);
    }

    return *this;
}

MyVector::~MyVector()
{
    delete[] _data;
}

std::size_t MyVector::size() const
{
    return _size;
}

std::size_t MyVector::capacity() const
{
    return _capacity;
}

void MyVector::set(std::size_t index, int value)
{
    if (index >= _size)
        return;

    _data[index] = value;
}

int MyVector::get(std::size_t index)
{
    if (index >= _size)
        throw std::runtime_error("Index out of range");

    return _data[index];
}

void MyVector::reserve(std::size_t new_capacity)
{
    if (new_capacity <= _capacity)
        return;

    int *new_data = new int[new_capacity];
    _capacity = new_capacity;
    std::copy(_data, _data + _size, new_data);

    delete[] _data;
    _data = new_data;
}

void MyVector::resize(std::size_t new_size)
{
    if (new_size <= _size)
    {
        _size = new_size;
        return;
    }

    if (new_size > _capacity)
    {
        _capacity = new_size;
        int *new_data = new int[_capacity];
        std::copy(_data, _data + _size, new_data);

        delete[] _data;
        _data = new_data;
    }

    memset(_data + _size, 0, (new_size - _size) * sizeof(int));
    _size = new_size;
}

void MyVector::push_back(int value)
{
    if (_size >= _capacity)
        reserve(_capacity * 2);

    _data[_size++] = value;
}

void MyVector::insert(std::size_t index, int value)
{
    if (index > _size)
        throw std::runtime_error("Index out of range");

    if (_size >= _capacity)
    {
        int *new_data = new int[_capacity * 2];

        std::copy(_data, _data + index, new_data);
        new_data[index] = value;
        _size++;

        if (index != _size - 1)
            std::copy(_data + index + 1, _data + _size, new_data + index + 1);

        return;
    }

    if (index != _size - 1)
        std::copy_backward(_data + index, _data + _size, _data + _size + 1);

    _data[index] = value;
    _size++;
}

void MyVector::erase(std::size_t index)
{
    if (index >= _size)
        throw std::runtime_error("Index out of range");

    if (index != _size - 1)
        std::copy(_data + index + 1, _data + _size, _data + index);

    _size--;
}
