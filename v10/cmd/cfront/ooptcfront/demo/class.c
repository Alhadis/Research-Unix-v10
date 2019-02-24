/* Test nested instantiations in the presence of multiple base classes */

					   
template <class p1, class p2> class b1 {
public:
  p1 x1 ;
  p2 y1 ;
  void foo() ;
} ;


template <class p1, class p2> class c {
public:
  p1 x2 ;
  p2 y2 ;
  void foo() ;
} ;

 

class c_arg {
public:
  int p ;
  int q ;
  inline foo (int i) { return (i == p); } ;
  c_arg() ;
} ;

c_arg::c_arg() {
  p = 1 ;
  q = 2 ;
} ;


int error = 0 ;

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
  c<int, c_arg> v1 ;
  check (v1.y2.p == 1) ;
  check (v1.y2.q == 2) ;
  check (v1.y2.foo(1)) ;  

  end_test(error) ;
}
