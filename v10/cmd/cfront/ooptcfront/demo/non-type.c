#include "check.h"

const int ap1 = 10 ;

int j ;

const int *ap2 = &ap1 ;


typedef int (*pf)() ;


const double ap4 = 1.0 ;


double p1 = 1000.0; // p1 should be blocked within the template defintion by the
                  // parameter 

template <class t1, int p2> class f {
public:
  t1 a[p2] ;
} ;

template <class t1, int p2> class f ;


// test out all the acceptable non-type argument types
template <class t1, class t2, class t3, class t4,
           int p1, int *p2, pf p3, double p4> class test
{
  t1 m1 ;
  t2 m2 ;
  t3 m3 ;
  t4 m4 ;

  t1 m5[p1] ;   // p1 should be acceptable as a parameter

  f<t1, p1> m6 ;  // test nested instantiations
	
public:
  test () ;
  int run(t1 fp1, t2 fp2, t3 fp3, t4 fp4) ;
  
};


template <class t1, class t2, class t3, class t4,
           int p1, int *p2, pf p3, double p4> test<t1,t2,t3,t4,p1,p2,p3,p4>::test() {
  m1 = p1 ;
  m2 = p2 ;
  m3 = p3 ; 
  m4 = p4 ;

}

int foo() {
  return (p1 == 1000.0) ;   // must be a reference to ::p1
} ;


template <class t1, class t2, class t3, class t4,
           int p1, int *p2, pf p3, double p4> int test<t1,t2,t3,t4,p1,p2,p3,p4>::run(t1 fp1, t2 fp2,t3 fp3, t4 fp4) {
  return ((m1 == fp1) &&
	  (m2 == fp2) &&
	  (m3 == fp3) &&
	  (m4 == fp4) &&
	  (*m3)()) ;
}


test<int, int *, pf, double, ap1, &j, &foo, 1.0 >  var ;

main() {
  start_test(__FILE__) ;
  
  check(var.run(ap1, &j, foo, 1.0)) ;

  // should be ::p1
  check(p1 == 1000.0) ;

  end_test() ;
}
