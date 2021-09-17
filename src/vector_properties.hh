#ifndef CHERRY_BLAZER_SRC_VECTOR_PROPERTIES_HH_
#define CHERRY_BLAZER_SRC_VECTOR_PROPERTIES_HH_

#include "util.hh"
#include "vector.hh"

#include <ostream>

namespace cherry_blazer {

// Basic properties such as comparison operators are defined here, so that it is not required to
// carry the whole set of templates of overload operators.

// TODO: this is essentially a copy-paste of vector_properties

// Vectors can be compared for equality.
template <typename T, std::size_t D>
constexpr auto operator==(Vector<T, D> const& lhs, Vector<T, D> const& rhs) noexcept {
    for (auto i{0U}; i < D; ++i) {
        if (!almost_equal(lhs[i], rhs[i]))
            return false;
    }
    return true;
}

// Vectors can be compared for inequality.
template <typename T, std::size_t D>
constexpr auto operator!=(Vector<T, D> const& lhs, Vector<T, D> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <typename T, std::size_t D>
constexpr auto& operator<<(std::ostream& os, Vector<T, D> const& v) noexcept {
    os << "{";
    for (auto i{0U}; i < D - 1; ++i)
        os << v[i] << ", ";
    return os << v[D - 1] << "}";
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR_PROPERTIES_HH_
