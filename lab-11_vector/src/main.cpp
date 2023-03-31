#include <iostream>

#include "my_vector.hpp"

namespace product {

class Product {
public:
    Product(const char* name, int quantity, double price);

private:
    char *name_;
    int quantity_;
    double price_;
};

}  // namespace product

int main() {
	containers::my_vector<int> v;
	v.push_back(2);
	const int n{3};
	v.push_back(n);
	std::cout << v << std::endl;

    //test_my_vector<int>(5, 10);
    //test_my_vector<Product>(Product("asdf", 4, 12.0), Product("qwe", -1, 7.5));

    return 0;
}
