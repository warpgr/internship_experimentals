#include <iostream>
#include <concepts>

/*static_assert(!std::is_same_v<K, bool> &&
        std::is_integral_v<T> || std::is_floating_point_v<T>);*/

/*template <typename T>
concept integral = std::is_integral_v<T>;*/

template <typename T>
concept Any = true;

template <typename T>
concept Number = !std::same_as<T, bool> &&
                  std::integral<T> || std::floating_point<T>;

template<typename T>
concept Arithmetical = requires(T a, T b) {
    {a + b} -> std::convertible_to<T>;   
    {a - b} -> std::convertible_to<T>;
    {a * b} -> std::convertible_to<T>;
    {a / b} -> std::convertible_to<T>;
};

template <Number T, Number K>
Number auto add(T a, K b) {
    return a + b;
}

auto mult(Arithmetical auto a, Arithmetical auto b) {
    return a * b;
}

int main() {
    add(1, 5.7);
    mult(4, 5.5);
}
