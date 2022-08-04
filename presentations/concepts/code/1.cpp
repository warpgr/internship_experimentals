#include <iostream>

template <typename T, typename K>
auto add(T a, K b) {
    return a + b;
}

int main() {
    add(1, 5.7);
    add(std::string("hello "), std::string("world\n"));
    add(true, 7);
}

