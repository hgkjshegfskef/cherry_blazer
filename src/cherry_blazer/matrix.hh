#pragma once

#include "detail/detail.hh"
#include "detail/matrix_base.hh"
#include "detail/types.hh"

namespace cherry_blazer {

// Any NxM matrix. Reuse base class routines.
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
class Matrix : public detail::MatrixBase<Precision, OuterDimension, InnerDimension> {
  public:
    Matrix() = default;
    using detail::MatrixBase<Precision, OuterDimension, InnerDimension>::MatrixBase;
};

// 1xM Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, 1, InnerDimension>;

} // namespace cherry_blazer

#include "matrix_properties.hh"
