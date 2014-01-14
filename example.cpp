#include "include/mylisp.hpp"
#include <boost/variant.hpp>
using namespace mylisp;

typedef boost::variant<std::string, char, bool> data;
std::ostream& operator << (std::ostream& os, data& d) {
    struct p : boost::static_visitor<void> {
        p(std::ostream& s) : os(s) {}
        std::ostream& os;
        void operator () (std::string& s) { os << s; }
        //void operator () (int i) { os << i; }
        void operator () (char c) { os << c; }
        void operator () (bool b) { os << (b ? "#t" : "#f"); }
    } print(os);
    boost::apply_visitor(print, d);
    return os;
}

int main() {
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

    std::cout << cons<data>(std::string("string"), cons<data>('c', cons<data>(bool(false), null))) << std::endl;
}

