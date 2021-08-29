#ifndef CHERRY_BLAZER_SRC_TUPLE_HH_
#define CHERRY_BLAZER_SRC_TUPLE_HH_

#include <type_traits>

struct Tuple {
    double x{};
    double y{};
    double z{};

    Tuple() = default;
    Tuple(double x, double y, double z);

    // Tuple += Tuple (= Tuple)
    Tuple& operator+=(Tuple const& rhs);

    // Tuple -= Tuple (= Tuple)
    Tuple& operator-=(Tuple const& rhs);

    // Tuples can be compared for equality.
    friend bool operator==(Tuple const& lhs, Tuple const& rhs);
};

static_assert(
    std::is_trivially_copyable_v<Tuple>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

// Tuple + Tuple = Tuple
Tuple operator+(Tuple lhs, Tuple const& rhs);

// Tuple - Tuple = Tuple
Tuple operator-(Tuple lhs, Tuple const& rhs);

// Tuples can be compared for inequality.
bool operator!=(Tuple const& lhs, Tuple const& rhs);

#endif // CHERRY_BLAZER_SRC_TUPLE_HH_
