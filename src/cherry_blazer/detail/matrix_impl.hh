#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <utility>

namespace cherry_blazer::detail {

template <std::size_t, typename T> using enumerate = T;

// Problem: need to be able to pass variadic pack of variadic packs of arguments with compile-time
// known length of every pack to Matrix ctor.
// Solution: this class.
// Special thanks to: wreien#0241, who wrote the first implementation with index_sequence.
template <typename Precision, typename NthInnerArrayIndexSequence, std::size_t InnerDimension>
class MatrixImpl;
template <typename Precision, std::size_t... NthInnerArrayPack, std::size_t InnerDimension>
class MatrixImpl<Precision, std::index_sequence<NthInnerArrayPack...>, InnerDimension> {
  public:
    MatrixImpl() = default;

    // Initialization is row-major. Every inner array is a row.
    constexpr explicit MatrixImpl(
        enumerate<NthInnerArrayPack,
                  Precision const (&)[InnerDimension]>... nth_inner_array) noexcept {
        // memcpy() is more efficient, but it is not constexpr.
        if (std::is_constant_evaluated()) {
            ((insert(mat_, nth_inner_array, NthInnerArrayPack)), ...);
        } else {
            ((std::memcpy(mat_.data() + NthInnerArrayPack * InnerDimension, nth_inner_array,
                          sizeof nth_inner_array)),
             ...);
        }
    }

  protected:
    std::array<Precision, sizeof...(NthInnerArrayPack) * InnerDimension>
        mat_; // NOLINT(readability-identifier-naming)

  public:
    // workaround: not possible to get mat_ type in child classes, because it is protected and
    // therefore not available in any context other than non-static member functions of the derived
    // class. Thus, it is necessary to type alias it here, then it is re-aliased in child class.
    using underlying_type = decltype(mat_);

  private:
    constexpr void insert(underlying_type& dst, Precision const (&src)[InnerDimension],
                          std::size_t nth_pack) noexcept {
        for (auto i{nth_pack * InnerDimension}, j{0UL};
             i < nth_pack * InnerDimension + InnerDimension; ++i, ++j)
            dst[i] = src[j];
    }
};

} // namespace cherry_blazer::detail
