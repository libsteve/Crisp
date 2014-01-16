#ifndef CONS_HPP
#define CONS_HPP

#include <boost/variant.hpp>
#include <ostream>
#include <functional>

namespace mylisp {

struct unit {} null;
inline std::ostream& operator << (std::ostream& os, unit& u) {
    return os << "()";
}

template <typename... T>
struct cons {
    typedef cons<T...> cons_t;
    typedef std::shared_ptr<cons_t> cons_ptr;
    typedef boost::variant<unit, cons_ptr, T...> value;

    // reference constructors
    cons(cons_t& a, cons_t& b) 
        : _car(std::make_shared<cons_t>(a)), 
          _cdr(std::make_shared<cons_t>(b)) {}
    cons(value a, cons_t& b) 
        : _car(a), 
          _cdr(cons_ptr(new cons_t(b))) {}
    cons(cons_t& a, value b)
        : _car(std::make_shared<cons_t>(a)),
          _cdr(b) {}

    // move constructors
    cons(cons_t&& a, cons_t&& b) 
        : _car(std::make_shared<cons_t>(a)), 
          _cdr(std::make_shared<cons_t>(b)) {}
    cons(value a, cons_t&& b) 
        : _car(a), 
          _cdr(cons_ptr(new cons_t(b))) {}
    cons(cons_t&& a, value b)
        : _car(std::make_shared<cons_t>(a)),
          _cdr(b) {}


    cons(value a, value b) : _car(a), _cdr(b) {}
    cons(value a) : _car(a), _cdr(null) {}
    cons(cons&& c) : _car(c._car), _cdr(c._cdr) {}
    cons(cons& c) : _car(c._car), _cdr(c._cdr) {}

    value& car() { return _car; }
    value& cdr() { return _cdr; }

private:
    value _car;
    value _cdr;

    //////
    // recursive definition for a static_visitor to print to an ostream
    // the template type must end with unit
    //   - because you can't have an explicit specialization within a class
    
    template <typename U, typename... UU>
    struct vprinter : vprinter<UU...> {
        vprinter(std::ostream& os) : vprinter<UU...>(os) {}
        using vprinter<UU...>::operator ();
        void operator () (U& u) { this->os << u; }
    };
    
    template <typename... UU>
    struct vprinter<unit, UU...> : boost::static_visitor<void> {
        std::ostream& os;
        vprinter(std::ostream& _os) : os(_os) {}
        void operator () (unit& u) { this->os << "()"; }
    };
    
    template <typename U, typename... UU>
    struct vprinter<std::shared_ptr<U>, UU...> : vprinter<UU...> {
        vprinter(std::ostream& os) : vprinter<UU...>(os) {}
        using vprinter<UU...>::operator ();
        void operator () (std::shared_ptr<U>& p) { this->os << *p; }
    };
    
    template <typename U, typename... UU>
    struct vprinter<U*, UU...> : vprinter<UU...> {
        vprinter(std::ostream& os) : vprinter<UU...>(os) {}
        using vprinter<UU...>::operator ();
        void operator () (U* p) { this->os << *p; }
    };


public:

    // ostream << operator for the value type
    friend std::ostream& operator << (std::ostream& os, value& v) {
        vprinter<T..., cons_ptr, unit> vp(os);
        boost::apply_visitor(vp, v);
        return os;
    }

    // ostream << operator for the cons<T> type
    friend std::ostream& operator << (std::ostream& os, cons_t& c) {
        bool isList = ({
            std::shared_ptr<cons_t> c_ptr(new cons_t(c));
            while(c_ptr->_cdr.which() == 1) {
                c_ptr = boost::get<std::shared_ptr<cons_t>>(c_ptr->_cdr);
            }
            bool result = (c_ptr->_cdr.which() == 0);
            result;
        });
        if (isList) {
            os << "(";
            std::shared_ptr<cons_t> c_ptr(new cons_t(c));
            while(c_ptr->_cdr.which() != 0) {
                os << c_ptr->_car << " ";
                c_ptr = boost::get<cons_ptr>(c_ptr->cdr());
            }
            os << c_ptr->_car << ")";
        } else {
            os << "(" << c.car() << " . " << c.cdr() << ")";
        }
        return os;
    }

    // ostream << operator for the cons<T> type
    // need to support the annoying move operator...
    friend std::ostream& operator << (std::ostream& os, cons_t&& c) {
        os << c;
        return os;
    }
};

}

#endif

