#include <ranges>
#include <iostream>
#include <vector>

using namespace std::views;

int main() {
    std::vector nums {1,2,3,4,5,6,7};
    auto odd = [](int i) { return i & 1; };
    auto square = [](int i) { return i * i; };

    for (const auto& i : transform(filter(nums, odd), square))
        std::cout << i << ' ';

    std::cout << '\n';

    auto result = iota(1, 10) | filter(odd)
                              | transform(square);

    for (const auto& i : result)
        std::cout << i << ' ';

    std::cout << '\n';
}
