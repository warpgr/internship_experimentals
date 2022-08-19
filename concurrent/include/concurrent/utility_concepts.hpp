#pragma once

namespace il {

template <typename Func, typename... Args>
concept Predicate = std::regular_invocable<Func, Args...> &&
                    std::convertible_to<std::invoke_result_t<Func, Args...>, bool>;

} // namespace li
