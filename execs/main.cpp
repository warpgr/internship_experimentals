#include <iostream>
#include <vector>
#include <cstdlib>

#include <conteiners/vector.hpp>

int main() {
    my::vector<int, my::allocator<int>> vec;
    vec.push_back(2);
    for (int i = 0; i < 30; ++i) {
        vec.push_back(i);
    }

    for (int i = 0; i < 30; ++i) {
        std::cout << vec[i] << " ";
    }

    return 0;
}