#include "include/cons.hpp"
#include "include/context.hpp"
using namespace mylisp;

int main() {
    std::cout << std::boolalpha;

    cons<std::string> f("hello", null);
    cons<std::string> d("world", f);
    cons<std::string> e("goodbye", d);
    std::cout << e << std::endl;
    
    cons<int> c(1, cons<int>(2, cons<int>(3, null)));
    std::cout << c << std::endl;

    std::cout << cons<int>(5, cons<int>(10, cons<int>(15, null))) << std::endl;
    std::cout << cons<int>(5, 10) << std::endl;
    std::cout << cons<int>(5, cons<int>(15, 10)) << std::endl;
    std::cout << cons<int>(cons<int>(null, cons<int>(10, cons<int>(15, null))), 5) << std::endl;

    std::cout << cons<std::string, char, bool>(std::string("string"), cons<std::string, char, bool>('c', cons<std::string, char, bool>(true, null))) << std::endl;
}

