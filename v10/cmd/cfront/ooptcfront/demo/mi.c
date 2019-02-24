/* Test nested instantiations in the presence of multiple base classes */

					   
template <class p1, class p2> class b1 {
public:
  p1 x1 ;
  p2 y1 ;
  void foo() ;
} ;


template <class p1, class p2> class b2 {
public:
  p1 x2 ;
  p2 y2 ;
  void foo() ;
} ;

template <class pd1, class pd2> class d :
                       public b1<pd1, pd2>, public b2<pd1, pd2> {
public:
  pd1 *i ;
  int check_i(pd1 test) ;
  int check_x(pd1 test) ;
  int check_y(pd2 test) ;
} ;


// Check access from within a member function
template  <class pd1, class pd2> int d<pd1,pd2>::check_i(pd1 test) {
  return (*i == test) ;
}


template  <class pd1, class pd2> int d<pd1,pd2>::check_x(pd1 test) {
  return ((x1 == test) && (x2 == test+1)) ;
}


template  <class pd1, class pd2> int d<pd1,pd2>::check_y(pd2 test) {
  return ((y1 == test) && (y2 == test+1)) ;
}
  
  
 
#define check(p)  if (!(p))printf("\nerror(%d): test failed at line %d",++error, __LINE__) 


typedef char *pc ;
typedef int *pi ;

extern "C" void printf(...) ;

extern "C" void exit(int) ;


void end_test(int error) 
{
  printf ("\n*End Testing %s", __FILE__) ;
  if (error)
    printf (" *** %d errors ***\n", error) ;
  else printf (" no errors detected\n") ;
  
  exit(error) ;
}

main () {
  printf ("\n*Begin Testing %s\n", __FILE__) ;
  d<int, pc> v1 ;
  int i ; char c ; int error = 0 ;

  v1.i = & i ;
  v1.x1 = i ;   v1.x2 = i + 1 ;
  v1.y1 = & c ; v1.y2 = &c + 1 ;

  check (v1.i == & i) ;
  check (v1.x1 == i) ;
  check(v1.y1 == & c) ;

  check (v1.check_i (i)) ;
  check (v1.check_x (i)) ;
  check (v1.check_y (&c)) ;

  end_test(error) ;

}
