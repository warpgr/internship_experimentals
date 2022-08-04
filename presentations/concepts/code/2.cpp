#include <iostream>
#include <type_traits>

template <typename T, typename K>
auto add(T a, K b) {
    static_assert(!std::is_same_v<T, bool> &&
        (std::is_integral_v<T> || std::is_floating_point_v<T>));
    static_assert(!std::is_same_v<K, bool> &&
        (std::is_integral_v<K> || std::is_floating_point_v<K>));

    return a + b;
}

/*template<>
auto add(int, bool) = delete;*/

int main() {
    add(1, 5.7);

    std::cout << add(8, true) << '\n';
    // static assertion fails
    add(std::string("hello "), std::string("world\n"));
    // static assertion fails
}
