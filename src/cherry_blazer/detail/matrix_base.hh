#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_PRIVATE_MATRIX_BASE_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_PRIVATE_MATRIX_BASE_HH_

#include "matrix_impl.hh"

#include <boost/assert.hpp>

#include <type_traits>

namespace cherry_blazer::detail {

// Base class with routines for any NxM Matrix.
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
class MatrixBase
    : public MatrixImpl<Precision, std::make_index_sequence<OuterDimension>, InnerDimension> {

    static_assert(std::is_same_v<Precision, float> || std::is_same_v<Precision, double>,
                  "Matrix only supports single and double precision.");

    static_assert(OuterDimension * InnerDimension != 0, "Both dimensions must be non-zero.");

  protected:
    using impl = MatrixImpl<Precision, std::make_index_sequence<OuterDimension>, InnerDimension>;
    using underlying_type = typename impl::underlying_type;

  public:
    static inline constexpr std::size_t outer_dimension = OuterDimension;
    static inline constexpr std::size_t inner_dimension = InnerDimension;

    MatrixBase() = default;
    using MatrixImpl<Precision, std::make_index_sequence<OuterDimension>,
                     InnerDimension>::MatrixImpl;

    // Reuse std::array interface.

    using value_type = Precision;
    using size_type = typename underlying_type::size_type;
    using difference_type = typename underlying_type::difference_type;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = typename underlying_type::iterator;
    using const_iterator = typename underlying_type::const_iterator;
    using reverse_iterator = typename underlying_type::reverse_iterator;
    using const_reverse_iterator = typename underlying_type::const_reverse_iterator;

    constexpr reference at(size_type outer_pos, size_type inner_pos) {
        return impl::mat_.at(outer_pos * InnerDimension + inner_pos);
    }
    [[nodiscard]] constexpr const_reference at(size_type outer_pos, size_type inner_pos) const {
        return impl::mat_.at(outer_pos * InnerDimension + inner_pos);
    }

    constexpr reference operator()(size_type outer_pos, size_type inner_pos) noexcept {
        BOOST_VERIFY(outer_pos < OuterDimension);
        BOOST_VERIFY(inner_pos < InnerDimension);
        return impl::mat_[outer_pos * InnerDimension + inner_pos];
    }
    [[nodiscard]] constexpr const_reference operator()(size_type outer_pos,
                                                       size_type inner_pos) const noexcept {
        BOOST_VERIFY(outer_pos < OuterDimension);
        BOOST_VERIFY(inner_pos < InnerDimension);
        return impl::mat_[outer_pos * InnerDimension + inner_pos];
    }

    constexpr reference front() { return impl::mat_.front(); }
    [[nodiscard]] constexpr const_reference front() const { return impl::mat_.front(); }

    constexpr reference back() { return impl::mat_.back(); }
    [[nodiscard]] constexpr const_reference back() const { return impl::mat_.back(); }

    constexpr pointer data() noexcept { return impl::mat_.data(); }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return impl::mat_.data(); }

    constexpr iterator begin() noexcept { return impl::mat_.begin(); }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return impl::mat_.begin(); }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return impl::mat_.cbegin(); }

    constexpr iterator end() noexcept { return impl::mat_.end(); }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return impl::mat_.end(); }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return impl::mat_.cend(); }

    constexpr reverse_iterator rbegin() noexcept { return impl::mat_.rbegin(); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
        return impl::mat_.rbegin();
    }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept {
        return impl::mat_.crbegin();
    }

    constexpr reverse_iterator rend() noexcept { return impl::mat_.rend(); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
        return impl::mat_.rend();
    }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept {
        return impl::mat_.crend();
    }

    [[nodiscard]] constexpr bool empty() const noexcept { return impl::mat_.empty(); }

    [[nodiscard]] constexpr size_type size() const noexcept { return impl::mat_.size(); }

    [[nodiscard]] constexpr size_type max_size() const noexcept { return impl::mat_.max_size(); }

    constexpr void fill(const_reference value) { return impl::mat_.fill(value); }

    constexpr void swap(MatrixBase& other) noexcept(std::is_nothrow_swappable_v<value_type>) {
        using std::swap;
        swap(impl::mat_, other.impl::mat_);
    }
};

} // namespace cherry_blazer::detail

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_PRIVATE_MATRIX_BASE_HH_
