// https://en.cppreference.com/w/cpp/algorithm/ranges/find

template<typename InputIt, typename UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate pred)
{
    for (; first != last; ++first) {
        if (pred(*first)) {
            return first;
        }
    }
    return last;
}

// constrained algorithm

struct find_fn {
  template< std::input_iterator I, std::sentinel_for<I> S, class T, class Proj = std::identity >
  requires std::indirect_binary_predicate<ranges::equal_to, std::projected<I, Proj>, 
                                          const T*>
  constexpr I operator()( I first, S last, const T& value, Proj proj = {} ) const
  {
      for (; first != last; ++first) {
          if (std::invoke(proj, *first) == value) {
              return first;
          }
      }
      return first;
  }
 
  template<ranges::input_range R, class T, class Proj = std::identity>
  requires std::indirect_binary_predicate<ranges::equal_to,
                                          std::projected<ranges::iterator_t<R>, Proj>,
                                          const T*>
  constexpr ranges::borrowed_iterator_t<R>
    operator()( R&& r, const T& value, Proj proj = {} ) const
  {
     return (*this)(ranges::begin(r), ranges::end(r), value, std::ref(proj));
  }
};


