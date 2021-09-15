#ifndef CHERRY_BLAZER_SRC_MATRIX_DETAIL_HH_
#define CHERRY_BLAZER_SRC_MATRIX_DETAIL_HH_

#include <iterator>

//namespace cherry_blazer::matrix::detail {
//
//template <typename T> class Iterator {
//  public:
//    using iterator_category = std::bidirectional_iterator_tag;
//    using difference_type = std::ptrdiff_t;
//    using value_type = T;
//    using pointer = value_type*;
//    using reference = value_type&;
//
//    explicit Iterator(pointer ptr) : ptr_{ptr} {}
//
//    reference operator*() { return *ptr_; }
//
//    pointer operator->() { return ptr_; }
//
//    Iterator& operator++() {
//        ptr_++;
//        return *this;
//    }
//
//    Iterator& operator--() {
//        ptr_--;
//        return *this;
//    }
//
//    Iterator operator++(int) { // NOLINT(cert-dcl21-cpp)
//        Iterator tmp = *this;
//        ++(*this); // omit infinite recursion
//        return tmp;
//    }
//
//    Iterator operator--(int) { // NOLINT(cert-dcl21-cpp)
//        Iterator tmp = *this;
//        --(*this); // omit infinite recursion
//        return tmp;
//    }
//
//    friend bool operator==(Iterator const& lhs, Iterator const& rhs) {
//        return lhs.ptr_ == rhs.ptr_;
//    };
//
//    friend bool operator!=(Iterator const& lhs, Iterator const& rhs) { return !(lhs == rhs); };
//
//  private:
//    pointer ptr_;
//};
//
//template <typename T> class ConstantIterator {
//  public:
//    using iterator_category = std::bidirectional_iterator_tag;
//    using difference_type = std::ptrdiff_t;
//    using value_type = T;
//    using pointer = value_type*;
//    using reference = value_type&;
//
//    explicit ConstantIterator(pointer ptr) : ptr_{ptr} {}
//
//    reference operator*() const { return *ptr_; }
//
//    pointer operator->() const { return ptr_; }
//
//    ConstantIterator& operator++() {
//        ptr_++;
//        return *this;
//    }
//
//    ConstantIterator& operator--() {
//        ptr_--;
//        return *this;
//    }
//
//    ConstantIterator operator++(int) { // NOLINT(cert-dcl21-cpp)
//        ConstantIterator tmp = *this;
//        ++(*this); // omit infinite recursion
//        return tmp;
//    }
//
//    ConstantIterator operator--(int) { // NOLINT(cert-dcl21-cpp)
//        ConstantIterator tmp = *this;
//        --(*this); // omit infinite recursion
//        return tmp;
//    }
//
//    friend bool operator==(ConstantIterator const& lhs, ConstantIterator const& rhs) {
//        return lhs.ptr_ == rhs.ptr_;
//    };
//
//    friend bool operator!=(ConstantIterator const& lhs, ConstantIterator const& rhs) {
//        return !(lhs == rhs);
//    };
//
//  private:
//    pointer ptr_;
//};
//
//} // namespace cherry_blazer::matrix::detail

#endif // CHERRY_BLAZER_SRC_MATRIX_DETAIL_HH_
