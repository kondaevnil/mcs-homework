#include "my_vector.hpp"
#include <cstring>

MyVector::MyVector()
{
    _data = new int[2];
    _capacity = 2;
    _size = 0;
}

MyVector::MyVector(std::size_t init_capacity)
{
    _data = new int[init_capacity];
    _capacity = init_capacity;
    _size = 0;
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
        return 0;

    return _data[index];
}

void MyVector::reserve(std::size_t new_capacity)
{
    if (new_capacity <= _capacity)
        return;

    int *new_data = new int[new_capacity];
    _capacity = new_capacity;
    memcpy(new_data, _data, sizeof(int) * _size);

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
        _capacity = new_size >= _capacity * 2 ? new_size : _capacity * 2;
        int *new_data = new int[_capacity];
        memcpy(new_data, _data, sizeof(int) * _size);

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
        return;

    if (_size >= _capacity)
        reserve(_capacity * 2);

    memmove(_data + index + 1, _data + index, sizeof(int) * (_size - index));

    _data[index] = value;
    _size++;
}

void MyVector::erase(std::size_t index)
{
    if (index >= _size)
        return;

    memmove(_data + index, _data + index + 1, sizeof(int) * (_size - index - 1));
    _size--;
}
