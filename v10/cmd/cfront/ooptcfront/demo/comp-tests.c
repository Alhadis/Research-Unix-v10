#include "check.h"


/* Compilation tests */


// There is some trickiness involved in cfront in dealing with member
// initializations, so check it.
class f ;

template <class T> class c {
public:
  const T f ;
  c() ;
} ;

template <class T> c<T>::c() : f(1) {}

c<int> x ;


// should able to define a class with a name that is identical to a type
// formal, they are in different scopes
class T {
} ;
T t1 ;



// it is ok to reference an as yet incomplete template class
template <class T> class t {
public:
  T m ;
  // references to an as yet incompletely defined template are ok
  t<double> * d  ;

  t() {}
} ;

t<int> tv ;


// check forward declarations of a template class

template <class T> class forward ;


template <class T> class backward {
public:
  T mem ;

  backward() { mem = 1.0 ; }
} ;

// shouldn't have any impact on the earlier template definition.
template <class T> class backward ;


class X {
public:
  forward<int> * p ;
  backward<double> *pb ;

  X() ;
} ;


template <class T> class forward {
public:
  T mem ;

  forward() { mem = 2 ; }
} ;


template <class T> class forward ;

X::X() {
  p = new forward<int> ; pb = new backward<double> ;
  check((p->mem == 2) && (pb->mem == 1.0)) ;
} 

X xv ;



main () {
  start_test(__FILE__) ;
  check(x.f == 1) ;

  end_test() ;
}
