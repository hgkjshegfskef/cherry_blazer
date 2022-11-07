This is my first attempt at CPU raytracing. I did not know anything about
computer graphics before starting this project. Therefore, I used Buck's "The
Ray Tracer Challenge" book to help me build something that resembles a ray
tracer. Many thanks for writing that book.

# Matrices, vectors and points

On this particular project I went somewhat over the top with the math library
that is supposed to provide matrices, vectors and points. I used many advanced
C++ techniques that I read about in the books, but never had a chance to try.
Funnily enough, I stumbled upon quite a few compiler bugs, incorrect
implementations of the standard or just different interpretation of the
standard between compilers.

In comparison to the rest of raytracing code, this is probably the highlight of
this project.

The math library is scattered under `src/cherry_blazer`.

The `Matrix` family of classes are the core of the implementation. See
`matrix_test.cc` for usage.

Problem: I want to be able to write
```cpp
constexpr Matrix m {{1., 2.}, {3., 4.}}; // 2x2 matrix
constexpr Matrix<double, 2, 2> m {{1., 2.}, {3., 4.}}; // equivalent to the above
constexpr Matrix m {{1,2,3}, {3,4}}; // shouldn't compile
```
So:
* matrix should deduce at compile time its size and precision
* matrix should support list initialization
* matrix should support constexpr construction

It all starts at `detail/matrix_impl.hh` with `MatrixImpl`. It is a class
template that is declared but not defined:

```cpp
template <typename Precision, typename NthInnerArrayIndexSequence, std::size_t InnerDimension>
class MatrixImpl;
```

`Precision` is either `float` or `double`, `InnerDimension` is the M in NxM
matrix. `NthInnerArrayIndexSequence` is an `std::index_sequence<0,1,2,...N-1>`
where `N` is the N in NxM matrix.

We then write an explicit class template specialization, which will be different for each index_sequence:

```cpp
template <typename Precision, std::size_t... NthInnerArrayPack, std::size_t InnerDimension>
class MatrixImpl<Precision, std::index_sequence<NthInnerArrayPack...>, InnerDimension> { /*...*/ }
```

And finally, because we cannot have a nested variadic parameter pack inside a
variadic parameter pack, in constructor we essentially take a pack of arrays
and use index_sequence to help us process the Nth array, so it is one at a
time:

```cpp
template <std::size_t, typename T> using enumerate = T;
//...
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
```
... and save the Nth array into the one-dimensional `std::array<Precision,
sizeof...(NthInnerArrayPack) * InnerDimension> mat_` that acts like a 2D array.

For constexprness we have to rely on a helper routine that takes an array.

We can now continue to build the abstraction with the `MatrixBase` class that
inherits from `MatrixImpl`, instantiating it with the correct index_sequence:

```cpp
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
class MatrixBase
    : public MatrixImpl<Precision, std::make_index_sequence<OuterDimension>, InnerDimension> {
  public:
    using MatrixImpl<Precision, std::make_index_sequence<OuterDimension>,
                     InnerDimension>::MatrixImpl; /* import base ctor */
};
```

The purpose of `MatrixBase` is to be the base class with routines for any NxM
matrix, e.g. iterators.

From here on out, the road is open to build more specific matrix classes. For example, any NxM matrix looks like this:

```cpp
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
class Matrix : public detail::MatrixBase<Precision, OuterDimension, InnerDimension> {
  public:
    using detail::MatrixBase<Precision, OuterDimension, InnerDimension>::MatrixBase; // just reuse base class routines
};
```

A square matrix (as a partial specialization):

```cpp

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension> class Matrix;

template <typename Precision, std::size_t Dimension>
class Matrix<Precision, Dimension, Dimension>
    : public detail::MatrixBase<Precision, Dimension, Dimension> {
  public:
    using detail::MatrixBase<Precision, Dimension, Dimension>::MatrixBase;
    // square matrix-specific routines
};
```

In order for compiler to deduct the types, we have to (somewhat manually) write the deduction guides:

```cpp
// 2x2 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>,
       detail::enumerate<1, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;
// needs another guide for 3x3, 4x4


// 1xM Matrix (vector)
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, 1, InnerDimension>;
```

Since vector can be thought of as an Nx1 matrix, we have a helper specialization:

```cpp
template <typename Precision, std::size_t Dimension>
class Matrix<Precision, Dimension, 1>
    : public detail::MatrixBase<Precision, Dimension + 1, 1> { /*...*/ };
```

and then, the vector itself:

```cpp
template <typename Precision, std::size_t Dimension>
class Vector : public Matrix<Precision, Dimension, 1> { /*...*/ }
```

Since we can only write a deduction guide for NxM matrix, where N has to be
known, we need something else to make `Vector v{{1.,2.,3.}};` ->
`Vector<double, 3>` -> `Matrix<double, 4, 1>` work. Well, let's just drop inner
list initialization alltogether and handle everything inside the variadic
constructor:

```cpp
template <typename... VectorComponents,
          typename = std::enable_if_t<
              // Do not shadow Vector's ctor from Points.
              not std::is_same_v<std::common_type_t<VectorComponents...>,
                                 std::decay_t<Point<Precision, Dimension>>> and
              // Do not shadow Vector's copy and move ctors.
              not(sizeof...(VectorComponents) == 1 and
                  std::is_same_v<std::common_type_t<VectorComponents...>,
                                 std::decay_t<Vector<Precision, Dimension>>>)>>
constexpr explicit Vector(VectorComponents&&... components) noexcept {
    impl::mat_ = {components...};
    // Last coordinate of the Vector is 0.
    impl::mat_[sizeof...(components)] = static_cast<Precision>(0);
}

template <typename First, typename... Rest,
          typename = std::enable_if_t<(std::is_same_v<First, Rest> && ...)>>
Vector(First, Rest...) -> Vector<First, 1 + sizeof...(Rest)>;
```

At the same time, vector is just like point: except that their last homogeneous
coordinate differs. Therefore, point is implemented with the help of vector, by
composition.
