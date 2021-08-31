#ifndef CHERRY_BLAZER_SRC_POINT_HH_
#define CHERRY_BLAZER_SRC_POINT_HH_

#include <ostream>
#include <type_traits>

namespace cherry_blazer {

struct Vector;
struct Point {
    double x{};
    double y{};
    double z{};

    Point() = default;
    Point(double x, double y, double z);
};

static_assert(
    std::is_trivially_copyable_v<Point>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

// -Point (= ERROR)

// Point*scalar (= ERROR)

// scalar*Point (= ERROR)

// Point/scalar (= ERROR)

// scalar/Point (= ERROR)

// Point += Point (= ERROR)

// Point + Point (= ERROR)

// Point -= Point (= ERROR)

// Point - Point = Vector (pointing from rhs to lhs)
Vector operator-(Point const& lhs, Point const& rhs);

// Points can be compared for equality.
bool operator==(Point const& lhs, Point const& rhs);

// Points can be compared for inequality.
bool operator!=(Point const& lhs, Point const& rhs);

std::ostream& operator<<(std::ostream& os, Point const& p);

// Vector-related operations:

// Point += Vector
Point& operator+=(Point& lhs, Vector const& rhs);

// Point + Vector = Point
Point operator+(Point lhs, Vector const& rhs);

// Point -= Vector
Point& operator-=(Point& lhs, Vector const& rhs);

// Point - Vector = Point (move backward by vector)
Point operator-(Point lhs, Vector const& rhs);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT_HH_
