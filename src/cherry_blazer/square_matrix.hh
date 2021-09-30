#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_SQUARE_MATRIX_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_SQUARE_MATRIX_HH_

#include "axis.hh"
#include "detail/matrix_base.hh"
#include "shearing.hh"
#include "vector_operations.hh"

#include <cmath>

namespace cherry_blazer {

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension> class Matrix;

// Square matrix. Reuse base class routines, and add new ones.
template <typename Precision, std::size_t Dimension>
class Matrix<Precision, Dimension, Dimension>
    : public detail::MatrixBase<Precision, Dimension, Dimension> {
  public:
    Matrix() = default;
    using detail::MatrixBase<Precision, Dimension, Dimension>::MatrixBase;

    enum class Type { Any, Identity, Translation, Scaling, Rotation, Shearing } type{Type::Any};

    // https://en.wikipedia.org/wiki/Identity_matrix
    [[nodiscard]] static constexpr auto identity() {
        Matrix<Precision, Dimension, Dimension> identity_matrix{};
        for (auto i{0U}; i < Dimension; ++i)
            identity_matrix(i, i) = static_cast<Precision>(1);
        identity_matrix.type = Type::Identity;
        return identity_matrix;
    }

    // For translation, scaling, reflection, rotation, shearing see:
    // https://en.wikipedia.org/wiki/Affine_transformation#Image_transformation

    // https://en.wikipedia.org/wiki/Translation_(geometry)
    [[nodiscard]] static constexpr auto
    translation(Matrix<Precision, Dimension - 1, 1> const& vec) {
        auto mat = identity();
        for (auto row{0U}; row < Dimension; ++row)
            mat(row, Dimension - 1) = vec[row];
        mat.type = Type::Translation;
        return mat;
    }

    // https://en.wikipedia.org/wiki/Scaling_(geometry)
    [[nodiscard]] static constexpr auto scaling(Matrix<Precision, Dimension - 1, 1> const& vec) {
        auto mat = identity();
        for (auto row{0U}; row < Dimension; ++row)
            mat(row, row) = vec[row];
        mat.type = Type::Scaling;
        return mat;
    }

    // https://en.wikipedia.org/wiki/Rotation_matrix
    // Rotation around X, Y, or Z axis.
    [[nodiscard]] static constexpr auto rotation(Axis const& axis, Precision const& radians) {
        auto const sine = std::sin(radians);
        auto const cosine = std::cos(radians);
        auto mat = identity();
        mat.type = Type::Rotation;
        switch (axis) {
        case Axis::X:
            mat(1, 1) = cosine;
            mat(1, 2) = -sine;
            mat(2, 1) = sine;
            mat(2, 2) = cosine;
            return mat;
        case Axis::Y:
            mat(0, 0) = cosine;
            mat(0, 2) = sine;
            mat(2, 0) = -sine;
            mat(2, 2) = cosine;
            return mat;
        case Axis::Z:
            mat(0, 0) = cosine;
            mat(0, 1) = -sine;
            mat(1, 0) = sine;
            mat(1, 1) = cosine;
            return mat;
        default:
            __builtin_unreachable();
        }
        __builtin_unreachable();
    }

    [[nodiscard]] static constexpr auto shearing(ShearDirection const& direction) {
        static_assert(Dimension == 4, "Only for 3D.");
        auto shearing_matrix = identity();
        std::visit(
            [&](auto&& kind) {
                using shear_kind = std::decay_t<decltype(kind)>;
                if constexpr (std::is_same_v<shear_kind, Shear::X::AgainstY>) {
                    shearing_matrix(0, 1) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::X::AgainstZ>) {
                    shearing_matrix(0, 2) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::Y::AgainstX>) {
                    shearing_matrix(1, 0) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::Y::AgainstZ>) {
                    shearing_matrix(1, 2) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::Z::AgainstX>) {
                    shearing_matrix(2, 0) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::Z::AgainstY>) {
                    shearing_matrix(2, 1) = static_cast<Precision>(1);
                } else {
                    static_assert(always_false_v<shear_kind>, "non-exhaustive visitor");
                }
            },
            direction);
        shearing_matrix.type = Type::Shearing;
        return shearing_matrix;
    }
};

// 2x2 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>,
       detail::enumerate<1, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

// 3x3 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>,
       detail::enumerate<1, Precision const (&)[InnerDimension]>,
       detail::enumerate<2, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

// 4x4 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>,
       detail::enumerate<1, Precision const (&)[InnerDimension]>,
       detail::enumerate<2, Precision const (&)[InnerDimension]>,
       detail::enumerate<3, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

using Mat2f = Matrix<float, 2, 2>;  // NOLINT(readability-identifier-naming)
using Mat3f = Matrix<float, 3, 3>;  // NOLINT(readability-identifier-naming)
using Mat4f = Matrix<float, 4, 4>;  // NOLINT(readability-identifier-naming)
using Mat2d = Matrix<double, 2, 2>; // NOLINT(readability-identifier-naming)
using Mat3d = Matrix<double, 3, 3>; // NOLINT(readability-identifier-naming)
using Mat4d = Matrix<double, 4, 4>; // NOLINT(readability-identifier-naming)

extern template class Matrix<double, 4, 4>;

} // namespace cherry_blazer

#include "matrix_properties.hh"

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_SQUARE_MATRIX_HH_
