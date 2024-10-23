#include <iostream>
#include "func_1.h"
int f(void) {
int var1 = 1;
int var2 {3};
std::cout << var1 + var2 << std::endl;
std::cout << var1 - var2 << std::endl;
std::cout << var1 / var2 << std::endl;
std::cout << var1 * var2 << std::endl;
return 0;
}
int main() {
 f();
}
