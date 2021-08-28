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
