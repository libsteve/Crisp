#ifndef MYLISP_HPP
#define MYLISP_HPP

#include <boost/variant.hpp>
#include <ostream>
#include <functional>

namespace mylisp {

struct unit {} null;

template <typename T>
struct cons {
    typedef std::shared_ptr<cons<T>> cons_ptr;
    typedef boost::variant<T, unit, cons_ptr> variant;

    // reference constructors
    cons(cons<T>& a, cons<T>& b) 
        : _car(std::make_shared<cons<T>>(a)), 
          _cdr(std::make_shared<cons<T>>(b)) {}
    cons(variant a, cons<T>& b) 
        : _car(a), 
          _cdr(cons_ptr(new cons<T>(b))) {}
    cons(cons<T>& a, variant b)
        : _car(std::make_shared<cons<T>>(a)),
          _cdr(b) {}

    // move constructors
    cons(cons<T>&& a, cons<T>&& b) 
        : _car(std::make_shared<cons<T>>(a)), 
          _cdr(std::make_shared<cons<T>>(b)) {}
    cons(variant a, cons<T>&& b) 
        : _car(a), 
          _cdr(cons_ptr(new cons<T>(b))) {}
    cons(cons<T>&& a, variant b)
        : _car(std::make_shared<cons<T>>(a)),
          _cdr(b) {}


    cons(variant a, variant b) : _car(a), _cdr(b) {}
    cons(variant a) : _car(a), _cdr(null) {}
    cons(cons&& c) : _car(c._car), _cdr(c._cdr) {}
    cons(cons& c) : _car(c._car), _cdr(c._cdr) {}

    variant& car() { return _car; }
    variant& cdr() { return _cdr; }

private:
    variant _car;
    variant _cdr;

public:
    friend std::ostream& operator << (std::ostream& os, variant& v) {
        switch(v.which()) {
            case 0:
                os << boost::get<T>(v);
                break;
            case 1:
                os << "()";
                break;
            case 2:
                os << *boost::get<cons_ptr>(v);
                break;
        }
        return os;
    }

    friend std::ostream& operator << (std::ostream& os, cons<T>& c) {
        bool isList = ({
            std::shared_ptr<cons<T>> c_ptr(new cons<T>(c));
            while(c_ptr->_cdr.which() == 2) {
                c_ptr = boost::get<std::shared_ptr<cons<T>>>(c_ptr->_cdr);
            }
            bool result = (c_ptr->_cdr.which() == 1);
            result;
        });
        if (isList) {
            os << "(";
            std::shared_ptr<cons<T>> c_ptr(new cons<T>(c));
            while(c_ptr->_cdr.which() != 1) {
                os << c_ptr->_car << " ";
                c_ptr = boost::get<cons_ptr>(c_ptr->cdr());
            }
            os << c_ptr->_car << ")";
        } else {
            os << "(" << c.car() << " . " << c.cdr() << ")";
        }
        return os;
    }

    // need to support the annoying move operator...
    friend std::ostream& operator << (std::ostream& os, cons<T>&& c) {
        os << c;
        return os;
    }
};

}

#endif

