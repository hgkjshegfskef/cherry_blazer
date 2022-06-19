#pragma once

#include "ray.hh"
#include "square_matrix.hh"

namespace cherry_blazer {

template <typename Derived, typename Precision, std::size_t Dimension>
class Transformation : public Matrix<Precision, Dimension, Dimension> {

    using base = Matrix<Precision, Dimension, Dimension>;

  protected:
    using typename base::impl;

  public:
    Transformation() { fill_identity(); };

    Ray transform(Ray const& ray) const noexcept { return as_derived().do_transform(ray); }

    // We want to pick up free templated functions which are defined for matrices (e.g. operator==).
    // But when looking for matching template, implicit conversions are not considered. Therefore,
    // we must upcast. Since Transformation doesn't contribute anything to the object layout (which
    // we check), we can safely do it.

    Matrix<Precision, Dimension, Dimension>& as_matrix() {
        // Sanity check: this class (or any child of Transformation) is convertible to Matrix.
        static_assert(std::is_nothrow_convertible_v<decltype(*this),
                                                    Matrix<Precision, Dimension, Dimension>>);
        // Sanity check: size of this class (or any child of Transformation) is same as Matrix (no
        // slicing is possible).
        static_assert(sizeof(Matrix<Precision, Dimension, Dimension>) == sizeof(decltype(*this)));

        return static_cast<Matrix<Precision, Dimension, Dimension>&>(*this);
    }

    Matrix<Precision, Dimension, Dimension> const& as_matrix() const {
        // Sanity check: this class (or any child of Transformation) is derived from Matrix.
        static_assert(std::is_nothrow_convertible_v<decltype(*this),
                                                    Matrix<Precision, Dimension, Dimension>>);
        // Sanity check: size of this class (or any child of Transformation) is same as Matrix (no
        // slicing is possible).
        static_assert(sizeof(Matrix<Precision, Dimension, Dimension>) == sizeof(decltype(*this)));

        return static_cast<Matrix<Precision, Dimension, Dimension> const&>(*this);
    }

  protected:
    void fill_zeroes() { impl::mat_ = {}; }

    void fill_identity() {
        fill_zeroes();
        for (auto row{0U}; row < Dimension; ++row)
            impl::mat_[row * Dimension + row] = static_cast<Precision>(1);
    }

  private:
    Derived& as_derived() { return static_cast<Derived&>(*this); }
    Derived const& as_derived() const { return static_cast<Derived const&>(*this); }
    friend Derived;
};

// template <typename Derived> class Transformer {
//   public:
//     template <typename Precision, std::size_t Dimension>
//     Ray transform(Ray const& ray,
//                   Matrix<Precision, Dimension, Dimension> const& mat) const noexcept {
//         return as_derived().do_transform(ray, mat);
//     }
//
//   private:
//     Derived& as_derived() { return static_cast<Derived&>(*this); }
//     Derived const& as_derived() const { return static_cast<Derived const&>(*this); }
//     friend Derived;
// };

// template <typename Derived, typename Precision, std::size_t Dimension>
// Ray transform(Transformer<Derived>& t, Ray const& ray,
//               Matrix<Precision, Dimension, Dimension> const& mat) noexcept {
//     return t.transform(ray, mat);
// }

template <typename Precision, std::size_t Dimension>
class Identity : public Transformation<Identity<Precision, Dimension>, Precision, Dimension> {

    using base = Transformation<Identity<Precision, Dimension>, Precision, Dimension>;

  protected:
    using typename base::impl;

  public:
    Identity() { base::fill_identity(); }
};

template <typename Precision, std::size_t Dimension>
class Translation : public Transformation<Translation<Precision, Dimension>, Precision, Dimension> {

    using base = Transformation<Translation<Precision, Dimension>, Precision, Dimension>;

  protected:
    using typename base::impl;

  public:
    Translation(Matrix<Precision, Dimension - 1, 1> const& vec) {
        base::fill_identity();
        for (auto row{0U}; row < Dimension; ++row)
            impl::mat_[row * Dimension + Dimension - 1] = vec[row];
    }

    Ray do_transform(Ray const& ray) const noexcept { return {*this * ray.origin, ray.direction}; }
};

template <typename Precision, std::size_t Dimension>
class Scaling : public Transformation<Scaling<Precision, Dimension>, Precision, Dimension> {

    using base = Transformation<Scaling<Precision, Dimension>, Precision, Dimension>;

  protected:
    using typename base::impl;

  public:
    Scaling(Matrix<Precision, Dimension - 1, 1> const& vec) {
        base::fill_identity();
        for (auto row{0U}; row < Dimension; ++row)
            impl::mat_[row * Dimension + row] = vec[row];
    }

    Ray do_transform(Ray const& ray) const noexcept {
        return {*this * ray.origin, *this * ray.direction};
    }
};

} // namespace cherry_blazer
