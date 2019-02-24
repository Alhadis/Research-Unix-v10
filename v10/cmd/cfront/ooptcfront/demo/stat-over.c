#include "check.h"
class c {

static int ov(char) ;  
static int ov(int) ;
} ;

int c::ov(char c) {return c == 1 ;} ;

int c::ov(int i) {return i == 2 ;} ;

   
template <class c1, class c2, class c3> class x {
public:
  int i ;
  int foo() ;
  
} ;

template <class c1, class c2, class c3>  int  x<c1,c2,c3>::foo() {
  c2 i = 2 ; c3 c = 1 ;

  return (c1::ov(i) && c1::ov(c)) ;
  
}

main () {
  start_test(__FILE__) ;
  
  x<c, int, char> y ;
  y.i = 0 ;

  check(y.foo()) ;

  end_test() ;

} 
