#include "check.h"

/* Test nested instantiations, when the instantiations are via base classes */
/* the test should compile and run without any errors */
					   
template <class p1, class p2> class b {
public:
  b(p1, p2) ;
  p1 x ;
  p2 y ;
  void foo() ;
} ;

template <class pd1, class pd2> class d : public b<pd1, pd2> {
public:
  d(pd1, pd2) ;
  pd1 *i ;
  int check_i(pd1 test) ;
  int check_x(pd1 test) ;
  int check_y(pd2 test) ;
} ;


// Check access from within a member function
template <class pd1, class pd2>int d<pd1,pd2>::check_i(pd1 test) {
  return (*i == test) ;
}

template <class pd1, class pd2>  d<pd1,pd2>::d(pd1 p1, pd2 p2) : (p1, p2) {
}

template <class pd1, class pd2> int d<pd1,pd2>::check_x(pd1 test) {
  return (x == test) ;
}


template <class pd1, class pd2> int d<pd1,pd2>::check_y(pd2 test) {
  return (y == test) ;
}
  
template  <class p1, class p2> b<p1,p2>::b(p1, p2 ) {
}
  
 

typedef char *pc ;
typedef int *pi ;



main () {
  start_test(__FILE__) ;

  {
    int i ; char c ;
      d<int, pc> v1(1, &c) ;


  v1.i = & i ;
  v1.x = i ;
  v1.y = & c ;

  check (v1.i == & i) ;
  check (v1.x == i) ;
  check(v1.y == & c) ;

  check (v1.check_i (i)) ;
  check (v1.check_x (i)) ;
  check (v1.check_y (&c)) ;

}

    {
      int i = 0 ; char c, *cc ;
        d<pc, int> v1(cc, i) ;


  v1.i = & cc  ;
  v1.x = &c ;
  v1.y =  i ;

  check (v1.i == &cc) ;
  check (v1.x == &c) ;
  check(v1.y == i) ;

  check (v1.check_i ( cc)) ;
  check (v1.check_x (&c)) ;
  check (v1.check_y (i)) ;

}

  

  end_test() ;
}
