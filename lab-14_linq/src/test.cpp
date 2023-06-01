#include "linq.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

using linq::from;

void from_select() {
    const int xs[] = { 1, 2, 3 };
    std::vector<int> res = from(xs, xs + 1).select([](int x) { return x + 5; }).to_vector();
    std::vector<int> expected = { 6 };
    assert(res == expected);
}

int main() {
  from_select();
  return 0;
}