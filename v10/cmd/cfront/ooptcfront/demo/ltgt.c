// just a simple check to verify that < and > operators are intact

#include <iostream.h>


template <class T>
class C
{
public:
  T x;

  C(T y) { x = y; }
  T f() { return x; }
  void m();
  int operator <(C&);
};


template <class T>
void C<T>::m()
{ cout << x << "\n"; }

template <class T>
int C<T>::operator <(C<T>& c)
{
  return x < c.x;
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

main()
{
  printf ("\n*Begin Testing %s\n", __FILE__) ;
  
  C<float> x(3.5), y(5.0);
  int error = 0 ;

  check( x < y) ;

  end_test(error) ;

}
