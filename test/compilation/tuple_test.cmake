# scalar/Tuple (= ERROR)
cherry_blazer_add_compilation_test(
    TupleTest_ScalarDividedByTuple
    [[
#include <tuple.hh>
int main() {
    cherry_blazer::Tuple t1{1, -2, 3};
    double scalar = 2;
    auto t2 = scalar / t1;
}
]])
