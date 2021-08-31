#ifndef CHERRY_BLAZER_SRC_TUPLE_HH_
#define CHERRY_BLAZER_SRC_TUPLE_HH_

#include <ostream>
#include <type_traits>

namespace cherry_blazer {

struct Tuple {
    double x{};
    double y{};
    double z{};

    Tuple() = default;
    Tuple(double x, double y, double z);
};

static_assert(
    std::is_trivially_copyable_v<Tuple>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

// -Tuple
Tuple operator-(Tuple const& t);

// Tuple*scalar
Tuple operator*(Tuple const& t, double scalar);

// scalar*Tuple
Tuple operator*(double scalar, Tuple const& t);

// Tuple/scalar
Tuple operator/(Tuple const& t, double scalar);

// scalar/Tuple (= ERROR)

// Tuple += Tuple (= Tuple)
Tuple& operator+=(Tuple& lhs, Tuple const& rhs);

// Tuple -= Tuple (= Tuple)
Tuple& operator-=(Tuple& lhs, Tuple const& rhs);

// Tuple + Tuple = Tuple
Tuple operator+(Tuple lhs, Tuple const& rhs);

// Tuple - Tuple = Tuple
Tuple operator-(Tuple lhs, Tuple const& rhs);

// Tuples can be compared for equality.
bool operator==(Tuple const& lhs, Tuple const& rhs);

// Tuples can be compared for inequality.
bool operator!=(Tuple const& lhs, Tuple const& rhs);

std::ostream& operator<<(std::ostream& os, Tuple const& t);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_TUPLE_HH_
