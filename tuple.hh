#ifndef CHERRY_BLAZER__TUPLE_HH_
#define CHERRY_BLAZER__TUPLE_HH_

#include <type_traits>

#include "util.hh"

struct Tuple {
    double x{};
    double y{};
    double z{};
    double w{};

    Tuple() = default;
    Tuple(double x_, double y_, double z_, double w_) : x{x_}, y{y_}, z{z_}, w{w_} {}

    [[nodiscard]] bool is_point() const { return w == 1.0; }
    [[nodiscard]] bool is_vector() const { return !is_point(); }

    Tuple& operator+=(Tuple const& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    friend Tuple operator+(Tuple lhs, Tuple const& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend bool operator==(Tuple const& l, Tuple const& r) {
        return almost_equal(l.x, r.x) && almost_equal(l.y, r.y) && almost_equal(l.z, r.z) &&
               almost_equal(l.w, r.w);
    }
};
static_assert(std::is_trivially_copyable_v<Tuple>);

#endif // CHERRY_BLAZER__TUPLE_HH_
