#ifndef MY_ARRAY_H_
#define MY_ARRAY_H_

#include <cstddef>
#include <stdexcept>

namespace utils
{
    void set_bit(std::byte &byte, std::size_t pos, bool value)
    {
        if (value == 1)
            byte |= (static_cast<std::byte>(1) << pos);
        else
            byte &= ~(static_cast<std::byte>(1) << pos);
    }

    bool get_bit(std::byte byte, std::size_t pos)
    {
        return static_cast<bool>(byte & static_cast<std::byte>(1 << pos));
    }
}

namespace lab_13
{
    template<typename T, std::size_t N>
    class my_array {
    private:
        T data_[N] {};
    public:
        const T & at(std::size_t index) const;
        const T & operator[](std::size_t index) const;

        T & at(std::size_t index);
        T & operator[](std::size_t index);

        [[nodiscard]] bool empty() const;
        [[nodiscard]] std::size_t size() const;

        void fill(const T &val);
    };

    template<typename T, std::size_t N>
    void my_array<T, N>::fill(const T &val)
    {
        for (std::size_t i = 0; i < N; i++)
        data_[i] = val;
    }

    template<typename T, std::size_t N>
    const T & my_array<T, N>::at(std::size_t index) const
    {
        if (index >= N)
            throw std::out_of_range("Index out of array range.");
        return data_[index];
    }

    template<typename T, std::size_t N>
    const T & my_array<T, N>::operator[](std::size_t index) const
    {
        return data_[index];
    }

    template<typename T, std::size_t N>
    T & my_array<T, N>::at(std::size_t index)
    {
        if (index >= N)
            throw std::out_of_range("Index out of array range.");
        return data_[index];
    }

    template<typename T, std::size_t N>
    T & my_array<T, N>::operator[](std::size_t index)
    {
        return data_[index];
    }

    template<typename T, std::size_t N>
    std::size_t my_array<T, N>::size() const
    {
        return N;
    }

    template<typename T, std::size_t N>
    bool my_array<T, N>::empty() const
    {
        return N == 0;
    }

    // for bool specialization
    class proxy_byte
    {
        std::byte &byte_;
        std::size_t bit_position_;
    public:
        proxy_byte() = delete;
        proxy_byte(std::byte &byte, std::size_t bit_position) : byte_(byte), bit_position_(bit_position) {}

        proxy_byte & operator=(bool value)
        {
            utils::set_bit(byte_, bit_position_, value);

            return *this;
        }

        proxy_byte & operator=(const proxy_byte &pb)
        {
            if (*this == pb)
                return *this;

            utils::set_bit(byte_, bit_position_, pb);

            return *this;
        }

        operator bool() const
        {
            return utils::get_bit(byte_, bit_position_);
        }
    };

    // specialization for bool
    template<std::size_t N>
    class my_array<bool, N>
    {
        std::byte data_[(N + 7) / 8] {};
    public:
        [[nodiscard]] bool empty() const;
        [[nodiscard]] std::size_t size() const;

        void fill(bool value);

        proxy_byte operator[](std::size_t index);
        bool operator[](std::size_t index) const;

        proxy_byte at(std::size_t index);
        [[nodiscard]] bool at(std::size_t index) const;
    };

    template<std::size_t N>
    proxy_byte my_array<bool, N>::at(std::size_t index)
    {
        if (index >= N)
            throw std::out_of_range("Index out of range");
        return proxy_byte(data_[index / 8], index % 8);
    }

    template<std::size_t N>
    bool my_array<bool, N>::at(std::size_t index) const
    {
        if (index >= N)
            throw std::out_of_range("Index out of range");
        return utils::get_bit(data_[index / 8], index % 8);
    }

    template<std::size_t N>
    proxy_byte my_array<bool, N>::operator[](std::size_t index)
    {
        return proxy_byte(data_[index / 8], index % 8);
    }

    template<std::size_t N>
    bool my_array<bool, N>::operator[](std::size_t index) const
    {
        return utils::get_bit(data_[index / 8], index % 8);
    }

    template<std::size_t N>
    void my_array<bool, N>::fill(bool value)
    {
        auto to_fill = static_cast<std::byte>(value ? 0xFF : 0x00);
        for (std::size_t i = 0; i < (N + 7) / 8; i++)
            data_[i] = to_fill;
    }

    template<std::size_t N>
    std::size_t my_array<bool, N>::size() const
    {
        return N;
    }

    template<std::size_t N>
    bool my_array<bool, N>::empty() const
    {
        return N == 0;
    }
}  // namespace lab_13

#endif  // MY_ARRAY_H_
