# scalar/Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_ScalarDividedByPoint
    [[
#include <point.hh>
int main() {
    Point p1{1, -2, 3};
    double scalar = 2;
    auto p2 = scalar / p1;
}
]])

# Point += Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointPlusEqualsPoint
    [[
#include <point.hh>
int main() {
    Point p1;
    Point p2;
    p1 += p2;
}
]])

# Point + Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointPlusPoint
    [[
#include <point.hh>
int main() {
    Point p1;
    Point p2;
    auto p3 = p1 + p2;
}
]])

# Point -= Point (= ERROR)
cherry_blazer_add_compilation_test(
    PointTest_PointMinusEqualsPoint
    [[
#include <point.hh>
int main() {
    Point p1;
    Point p2;
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
    Point p;
    Vector v;
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
    Point p;
    Vector v;
    p != v;
}
]])
