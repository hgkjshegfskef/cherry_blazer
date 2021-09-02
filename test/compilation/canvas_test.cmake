# Canvas is not default constructable
cherry_blazer_add_compilation_test(
    CanvasTest_CanvasNotDefaultConstructable
    [[
#include <canvas.hh>
int main() {
    cherry_blazer::Canvas c;
}
]])
