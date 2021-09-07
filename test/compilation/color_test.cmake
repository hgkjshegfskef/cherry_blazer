# Partial ctors should not compile
cherry_blazer_add_compilation_test(
    ColorTest_PartialCtorsOnlyOneArgument
    [[
#include <color.hh>
int main() {
    cherry_blazer::Color c{1};
}
]])

cherry_blazer_add_compilation_test(
    ColorTest_PartialCtorsOnlyTwoArguments
    [[
#include <color.hh>
int main() {
    cherry_blazer::Color c{1, 2};
}
]])

# scalar/Color (= ERROR)
cherry_blazer_add_compilation_test(
    ColorTest_ScalarDividedByColor
    [[
#include <color.hh>
int main() {
    cherry_blazer::Color c{1, -2, 3};
    double scalar = 2;
    scalar / v1;
}
]])
