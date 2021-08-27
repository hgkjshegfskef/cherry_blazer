#ifndef CHERRY_BLAZER__TUPLE_HH_
#define CHERRY_BLAZER__TUPLE_HH_

#include <type_traits>

#include "util.hh"

struct Tuple {
    double x{};
    double y{};
    double z{};

    Tuple() = default;
    Tuple(double x_, double y_, double z_) : x{x_}, y{y_}, z{z_} {}

    // Tuple += Tuple (= Tuple)
    Tuple& operator+=(Tuple const& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // Tuple + Tuple = Tuple
    friend Tuple operator+(Tuple lhs, Tuple const& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend bool operator==(Tuple const& l, Tuple const& r) {
        // floating-point comparison through epsilon
        return almost_equal(l.x, r.x) && almost_equal(l.y, r.y) && almost_equal(l.z, r.z);
    }
};

static_assert(
    std::is_trivially_copyable_v<Tuple>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

#endif // CHERRY_BLAZER__TUPLE_HH_
