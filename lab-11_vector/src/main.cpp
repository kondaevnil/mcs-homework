#include <iostream>

#include "my_vector.h"

#include <cstring>

namespace product
{
    class Product
    {
    public:
        Product(const char* name, int quantity, double price);
        Product(Product const &other);
        ~Product();

        bool operator==(const Product &other);

        friend std::ostream & operator<<(std::ostream &os, Product &p);

    private:
        char *name_;
        int quantity_;
        double price_;
    };

    Product::Product(const char *name, int quantity, double price) : quantity_(quantity), price_(price)
    {
        std::size_t len = strlen(name);

        name_ = new char[len + 1];
        std::copy(name, name + len + 1, name_);
    }

    Product::~Product()
    {
        delete[] name_;
    }

    std::ostream &operator<<(std::ostream &os, Product &p)
    {
        for (std::size_t i = 0; i < strlen(p.name_); i++)
            os << p.name_[i];
        os << " " << p.quantity_ << " " << p.price_;

        return os;
    }

    Product::Product(const Product &other)
    {
        quantity_ = other.quantity_;
        price_ = other.price_;
        std::size_t len = strlen(other.name_);

        name_ = new char[len + 1];
        std::copy(other.name_, other.name_ + len + 1, name_);
    }

    bool Product::operator==(const Product &other)
    {
        return strcmp(name_, other.name_) == 0 && quantity_ == other.quantity_ && price_ == other.price_;
    }

}

template<typename T>
void test_my_vector(T *array, std::size_t n)
{
    containers::my_vector<T> v;

    for (std::size_t i = 0; i < n; i++)
        v.push_back(array[i]);

    assert(v.size() == n);

    for (std::size_t i = 0; i < n; i++)
        assert(v[i] == array[i]);

    std::size_t pow2 = 1;
    while (pow2 < n)
        pow2 *= 2;

    assert(v.capacity() == pow2);

    v.reserve(n * 2);
    assert(v.capacity() == pow2 * 2);
    assert(v.size() == n);

    v.pop_back();
    assert(v.size() == n - 1);

    assert(!v.empty());
    v.clear();
    assert(v.empty());

    containers::my_vector<T> u(v);
    assert(u.size() == v.size());
    assert(u.capacity() == v.capacity());
    for (std::size_t i = 0; i < v.size(); i++)
        assert(u[i] == v[i]);
}

template<typename T>
void test_my_vector_default_constructible()
{
    const std::size_t def_size = 10;

    containers::my_vector<T> v(def_size);
    for (std::size_t i = 0; i < def_size; i++)
        assert(v[i] == T());

    v.resize(def_size / 2);
    assert(v.size() == def_size / 2);

    for (std::size_t i = 0; i < def_size / 2; i++)
        assert(v[i] == T());

    v.resize(def_size);
    assert(v.size() == def_size);

    for (std::size_t i = 0; i < def_size; i++)
        assert(v[i] == T());
}


int main()
{
    int v[] = {5, 1, 2, 5, 4, 2, 0, 6, 3};
    test_my_vector<int>(v, sizeof(v) / sizeof(int));

    product::Product u[] =
    {
            product::Product("asdf", 4, 12.0),
            product::Product("qwe", -1, 12.5),
            product::Product("qwe", 1, 714.5),
            product::Product("qfsdf", 4, 71.5),
            product::Product("qgdfgdfs", 2, 91.5)
    };
    test_my_vector<product::Product>(u, sizeof(u) / sizeof(product::Product));

    test_my_vector_default_constructible<int>();

    return 0;
}
