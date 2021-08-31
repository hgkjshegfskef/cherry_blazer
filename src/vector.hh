#ifndef CHERRY_BLAZER_SRC_VECTOR_HH_
#define CHERRY_BLAZER_SRC_VECTOR_HH_

#include <ostream>
#include <type_traits>

namespace cherry_blazer {

struct Point;
struct Vector {
    double x{};
    double y{};
    double z{};

    Vector() = default;
    Vector(double x, double y, double z);
    Vector(Point const& begin, Point const& end);
};

static_assert(
    std::is_trivially_copyable_v<Vector>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

// -Vector
Vector operator-(Vector const& v);

// Vector*scalar
Vector operator*(Vector const& v, double scalar);

// scalar*Vector
Vector operator*(double scalar, Vector const& v);

// Vector/scalar
Vector operator/(Vector const& v, double scalar);

// scalar/Vector (= ERROR)

// Vector += Vector (= Vector)
Vector& operator+=(Vector& lhs, Vector const& rhs);

// Vector -= Vector (= Vector)
Vector& operator-=(Vector& lhs, Vector const& rhs);

// Vector + Vector = Vector
Vector operator+(Vector lhs, Vector const& rhs);

// Vector - Vector = Vector
Vector operator-(Vector lhs, Vector const& rhs);

// Vectors can be compared for equality.
bool operator==(Vector const& lhs, Vector const& rhs);

// Vectors can be compared for inequality.
bool operator!=(Vector const& lhs, Vector const& rhs);

std::ostream& operator<<(std::ostream& os, Vector const& v);

// Point-related operations:

// Vector += Point (= ERROR)

// Vector + Point = Point
Point operator+(Vector const& lhs, Point rhs);

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Freestanding operations:

double magnitude(Vector const& v);

Vector normalize(Vector const& v);

double dot(Vector const& v1, Vector const& v2);

Vector cross(Vector const& v1, Vector const& v2);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR_HH_
