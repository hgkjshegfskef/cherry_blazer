# scalar*Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_ScalarTimesPoint
    [[
#include <point.hh>
int main() {
    cherry_blazer::Point p;
    double scalar{};
    scalar * p;
}
]])

# Point*scalar (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointTimesScalar
    [[
#include <point.hh>
int main() {
    cherry_blazer::Point p;
    double scalar{};
    p * scalar;
}
]])

# scalar/Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_ScalarDividedByPoint
    [[
#include <point.hh>
int main() {
    cherry_blazer::Point p;
    double scalar{};
    scalar / p;
}
]])

# Point/scalar (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointDividedByScalar
    [[
#include <point.hh>
int main() {
    cherry_blazer::Point p;
    double scalar{};
    p / scalar;
}
]])

# Point += Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointPlusEqualsPoint
    [[
#include <point.hh>
int main() {
    cherry_blazer::Point p1;
    cherry_blazer::Point p2;
    p1 += p2;
}
]])

# Point + Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointPlusPoint
    [[
#include <point.hh>
int main() {
    cherry_blazer::Point p1;
    cherry_blazer::Point p2;
    p1 + p2;
}
]])

# Point -= Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointMinusEqualsPoint
    [[
#include <point.hh>
int main() {
    cherry_blazer::Point p1;
    cherry_blazer::Point p2;
    p1 -= p2;
}
]])

# Vector-related:

# Point == Vector (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointEqualsVector
    [[
#include <point.hh>
#include <vector.hh>
int main() {
    cherry_blazer::Point p;
    cherry_blazer::Vector v;
    p == v;
}
]])

# Point != Vector (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointNotEqualsVector
    [[
#include <point.hh>
#include <vector.hh>
int main() {
    cherry_blazer::Point p;
    cherry_blazer::Vector v;
    p != v;
}
]])
