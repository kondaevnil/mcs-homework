#ifndef LAB_11_VECTOR_MY_VECTOR_IMPL_H
#define LAB_11_VECTOR_MY_VECTOR_IMPL_H

#include <cstddef>
#include <stdexcept>
#include <ostream>

namespace containers
{
    static const size_t my_vector_default_capacity = 1;

    template<typename T>
    std::ostream & operator<<(std::ostream &os, const my_vector<T> &vec)
    {
        for (std::size_t i = 0; i < vec.size(); i++)
            os << vec.array_[i] << (i == vec.size() - 1 ? '\n' : ' ');

        return os;
    }

    template<typename T>
    my_vector<T>::my_vector()
    {
        capacity_ = my_vector_default_capacity;
        size_ = 0;
        array_ = reinterpret_cast<T *>(new char[sizeof(T) * capacity_]);
    }

    template<typename T>
    my_vector<T>::my_vector(std::size_t n)
    {
        capacity_ = my_vector_default_capacity;

        while (capacity_ < n)
            capacity_ *= 2;
        
        size_ = n;
        array_ = reinterpret_cast<T *>(new char[sizeof(T) * capacity_]);

        for (int i = 0; i < size_; i++)
            new (array_ + i) T();
    }

    template<typename T>
    my_vector<T>::~my_vector()
    {
        for (int i = 0; i < size_; i++)
            array_[i].~T();
        operator delete[](array_);
    }

    template<typename T>
    my_vector<T>::my_vector(const my_vector<T> &other)
    {
        capacity_ = other.capacity_;
        size_ = other.size_;
        array_ = reinterpret_cast<T *>(new char[sizeof(T) * capacity_]);

        for (std::size_t i = 0; i < size_; i++)
            new (array_ + i) T(other.array_[i]);
    }

    template<typename T>
    my_vector<T> & my_vector<T>::operator=(my_vector<T> other)
    {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(array_, other.array_);

        return *this;
    }

    template<typename T>
    std::size_t my_vector<T>::size() const
    {
        return size_;
    }

    template<typename T>
    bool my_vector<T>::empty() const
    {
        return size_ == 0;
    }

    template<typename T>
    std::size_t my_vector<T>::capacity() const
    {
        return capacity_;
    }

    template<typename T>
    void my_vector<T>::resize(std::size_t n)
    {
        if (size_ <= n)
        {
            for (std::size_t i = n; i < size_; i++)
                array_[i].~T();
            size_ = n;
            return;
        }

        if (n > capacity_)
            reserve(n);

        for (std::size_t i = size_; i < n; i++)
            new (array_ + i) T();
        size_ = n;
    }

    template<typename T>
    void my_vector<T>::reserve(std::size_t n)
    {
        if (n <= capacity_)
            return;

        T *new_arr = reinterpret_cast<T *>(new char[sizeof(T) * n]);

        while (capacity_ < n)
            capacity_ *= 2;

        for (std::size_t i = 0; i < size_; i++)
        {
            new (new_arr + i) T(array_[i]);
            array_[i].~T();
        }

        operator delete[](array_);

        array_ = new_arr;
    }

    template<typename T>
    T & my_vector<T>::operator[](std::size_t index) const
    {
        if (index >= size_)
            throw std::runtime_error("Index out of range.");

        return array_[index];
    }


    template<typename T>
    void my_vector<T>::push_back(T t)
    {
        if (size_ >= capacity_)
            reserve(capacity_ * 2);

        new (array_ + size_++) T(t);
    }

    template<typename T>
    void my_vector<T>::pop_back()
    {
        if (size_ == 0)
            return;

        array_[--size_].~T();
    }

    template<typename T>
    void my_vector<T>::clear()
    {
        while (size_ != 0)
            pop_back();
    }
}

#endif