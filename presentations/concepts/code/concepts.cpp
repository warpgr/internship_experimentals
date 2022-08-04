#include <concepts>

template<typename T, typename U>
concept weakly_equality_comparable_with = 
requires(const remove_reference<T>& a, const remove_reference<U>& b) {
    { a == b } -> std::same_as<bool>;
    { a != b } -> std::same_as<bool>;
    { b == a } -> std::same_as<bool>;
    { b != a } -> std::same_as<bool>;
};

template<class I>
  concept random_access_iterator =
    std::bidirectional_iterator<I> &&
    std::derived_from<<I>, std::random_access_iterator_tag> &&
    std::totally_ordered<I> &&
    std::sized_sentinel_for<I, I> &&
    requires(I i, const I j, const std::iter_difference_t<I> n) {
      { i += n } -> std::same_as<I&>;
      { j +  n } -> std::same_as<I>;
      { n +  j } -> std::same_as<I>;
      { i -= n } -> std::same_as<I&>;
      { j -  n } -> std::same_as<I>;
      {  j[n]  } -> std::same_as<std::iter_reference_t<I>>;
    };

template <typename BidirectionalIterator>
...
template <std::bidirectional_iterator It>
...

template<class BidirIt>
void my_reverse(BidirIt first, BidirIt last)
{
    typename std::iterator_traits<BidirIt>::difference_type n 
        = std::distance(first, last);
    for (--n; n > 0; n -= 2) {
        typename std::iterator_traits<BidirIt>::value_type tmp = *first;
        *first++ = *--last;
        *last = tmp;
    }
}

template <class T>
concept movable =
  std::is_object_v<T> &&
  std::move_constructible<T> &&
  std::assignable_from<T&, T> &&
  std::swappable<T>;
