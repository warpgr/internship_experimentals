#pragma once 


namespace my {

template <typename T>
class list_allocator {
// TODO:
};

template <typename T, typename Alloc = list_allocator<T>>
class list {
    Alloc allocator_;
// TODO:
};
}