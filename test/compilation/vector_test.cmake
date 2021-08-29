# Point-related:

# Vector += Point (= ERROR)
cherry_blazer_add_compilation_test(
    VectorTest_VectorPlusEqualsPoint
    [[
#include <point.hh>
#include <vector.hh>
int main() {
    Point p;
    Vector v;
    v += p;
}
]])

# Vector -= Point (= ERROR)
cherry_blazer_add_compilation_test(
    VectorTest_VectorMinusEqualsPoint
    [[
#include <point.hh>
#include <vector.hh>
int main() {
    Point p;
    Vector v;
    v -= p;
}
]])

# Vector - Point (= ERROR)
cherry_blazer_add_compilation_test(
    VectorTest_VectorMinusPoint
    [[
#include <point.hh>
#include <vector.hh>
int main() {
    Point p;
    Vector v;
    v - p;
}
]])

# Vector == Point (= ERROR)
cherry_blazer_add_compilation_test(
    VectorTest_VectorEqualsPoint
    [[
#include <point.hh>
#include <vector.hh>
int main() {
    Point p;
    Vector v;
    v == p;
}
]])

# Vector != Point (= ERROR)
cherry_blazer_add_compilation_test(
    VectorTest_VectorNotEqualsPoint
    [[
#include <point.hh>
#include <vector.hh>
int main() {
    Point p;
    Vector v;
    v != p;
}
]])
