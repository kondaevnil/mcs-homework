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
}



int main()
{
	containers::my_vector<int> v(10);

//	v.push_back(2);
//	const int n{3};
//	v.push_back(n);
//	std::cout << v << std::endl;

    //test_my_vector<int>(5, 10);
    //test_my_vector<Product>(Product("asdf", 4, 12.0), Product("qwe", -1, 7.5));

    return 0;
}
