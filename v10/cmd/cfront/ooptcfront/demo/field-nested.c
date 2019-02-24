#include "check.h"
// Test nested instantiations, when the instantiations are fields within the
// the class
					   
template <class p1, class p2> class b {
public:
  p1 x ;
  p2 y ;
  void foo() ;
} ;

template <class pd1, class pd2> class d {
public:
  pd1 *i ;
  void foo() ;
  b<pd1, pd2> bv ;
} ;


typedef char *pc ;
typedef int *pi ;


main () {
  start_test(__FILE__) ;
  d<int, pc> v1 ;
  int i ; char c ; int error = 0 ;

  v1.i = & i ;
  v1.bv.x = i ;
  v1.bv.y = & c ;
  check(v1.i == & i) ;
  check(v1.bv.x == i) ;
  check(v1.bv.y == & c) ;

  end_test() ;
}
