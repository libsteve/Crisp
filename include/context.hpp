#ifndef CRISP_CONTEXT_HPP
#define CRISP_CONTEXT_HPP

#include "cons.hpp"
#include <functional>

namespace crisp {

template <typename... T>
struct context {
    using cons_t = typename cons<T...>::cons_t;
    using cons_ptr = typename cons<T...>::cons_ptr;
    using value = typename cons<T...>::value;
    using whichValue = typename cons<T...>::whichValue;

    #define make_cons std::make_shared<cons_t>
    #define get_cons boost::get<cons_ptr>

    context() : _context(make_cons(cons_t(null))) {}

    void enterScope() {
        _context = make_cons(cons_t(null, _context));
    }

    void exitScope() {
        // we cannot exit the global scope
        if (_context->cdr().which() == cons_t::isCons) {
            _context = get_cons(_context->cdr());
        }
    }

    value& addKeyValue(value& k, value& v) {
        cons_t key_value_pair = cons_t(k, cons_t(v, null));
        cons_t scope = cons_t(key_value_pair, cons_t(_context->car(), null));
        _context = make_cons(cons_t(scope, _context->cdr()));
        return v;
    }

    template <typename key_t>
    cons_ptr& findKeyValuePair(value& key) {
        auto get_value = [](value& v) { return get_cons(v)->car(); };
        auto get_next = [](value& v) { return get_cons(v)->cdr(); };
        auto get = [](value& v) { return boost::get<key_t>(v); };

        /*// look in every scope in the context
        value _ctx = _context;
        while (_ctx.which() == cons_t::isCons) {
            // look in every pair in the scope
            value scope = get_value(_ctx);
            while (scope.which() == cons_t::isCons) {
                value& pair = get_value(scope);
                if (pair.which() == cons_t::isCons
                    && get_value(pair).which() == key.which()
                    && get(get_value(pair)) == get(key))
                    return get_cons(pair);
                }
                scope = get_next(scope);
            }
            _ctx = get_next(_ctx);
        }*/

        cons_ptr result;
        _context->foreach([&](value& scope) {
            if (scope.which() == cons_t::isCons) {
                return get_cons(scope)->foreach([&](value& pair) {
                    if (pair.which() == cons_t::isCons
                        && get_value(pair).which() == key.which()
                        && get(get_value(pair)) == get(key)) {
                        result = get_cons(pair);
                        return true;
                    }
                    return false;
                });
            }
            return false;
        });
        return result; // null shared pointer
    }

    template <typename key_t>
    struct NoKeyException : std::exception {
        NoKeyException(key_t& k) : _key(k) {}
        
        friend std::ostream& operator << (std::ostream& os, 
                                          NoKeyException<key_t>& e) {
            return os << "No value for key \"" << e._key << "\" in scope.";
        }

        virtual const char *what() const throw() {
            return "No value for key found.";
        }

    private:
        key_t _key;
    };

    template <typename key_t>
    value& setKeyValue(value& k, value& v) throw (NoKeyException<key_t>) {
        cons_ptr& pair = findKeyValuePair<key_t>(k);
        if (not pair) throw NoKeyException<key_t>(k);
        get_cons(get_cons(pair)->cdr())->setCar(v);
        return v;
    }

    template <typename key_t>
    value& getKeyValue(value& k) throw (NoKeyException<key_t>) {
        cons_ptr& pair = findKeyValuePair<key_t>(k);
        if (not pair) throw NoKeyException<key_t>(k);
        return get_cons(get_cons(pair)->cdr())->car();
    }

    #undef make_cons
    #undef get_cons

private:
    cons_ptr _context;
};

}

#endif

