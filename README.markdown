#CRISP

######CRappy lISP

A crappy implementation of some sort of lisp variant.

Crisp is an attempt at making a generic implementation of a cons type in C++
using boost::variant types for allowing multiple types of data to exist within
a cons pair.

##What is a cons pair?

A cons pair is a simple pairing of two values. Traditionally, linked lists are
made with cons pairs by having a value in the "car" or head of the cons pair, 
and either a "null" or another cons value in the tail of the cons pair.

##How is the cons pair executed?

Cons pair lists can be used to represent functions and data structures.

Scope for functions and variable parameters is kept track by a context object
that uses cons pairs to store key-value pairs of data. The context can enter
a new scope and leave old scopes, add new key-value pairs to the current scope,
get values from key-value pairs by looking up through the scope, and set values
for key-value pairs already added to the context.

This context can be used in conjunction with an execution mechanism to read
cons pair lists representing programs and functions, and execute them, keeping
proper scoping for variables and values.

##Goals for writing Crisp

- To write a generic implementation of the cons pair in C++11
- To better understand C++11 with templates and generic programming
- To write a runtime for some sort of lisp implementation
- To better understand some of the aspects of lisp implementations

