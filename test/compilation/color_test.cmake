# scalar/Color (= ERROR)
cherry_blazer_add_compilation_test(
    ColorTest_ScalarDividedByColor
    [[
#include <vector.hh>
int main() {
    cherry_blazer::Color v1{1, -2, 3};
    double scalar = 2;
    auto v2 = scalar / v1;
}
]])
