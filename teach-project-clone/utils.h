#include <iterator> 
#include <sstream>

template<class NumType>
int sign(NumType num) {
    return num < 0 ? -1 : num > 0;
}

template<class Iterator, class RNG>
Iterator choice(Iterator beg, Iterator end, RNG& rng) {
    size_t dst = std::distance(beg, end);
    size_t pos = rng() % dst;
    return std::next(beg, pos);
}

std::istringstream stdin_line_stream();
