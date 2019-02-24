#include "check.h"

// check overriding of member functions


template <class T> class oc {
public:
  T mem ;
  // a vanilla function
  foo() ;
  // an operator
  operator + (oc) ;
  // a constructor
  oc() ;
} ;

// default class definitions
template <class T> oc<T>::foo() {return 1;}
template <class T> oc<T>::oc() {mem = 1;}
template <class T> oc<T>::operator + (oc<T>) {return 1;}

// override them with the real definitions, for int instantiations
oc<int>::foo() {return 2;}
oc<int>::operator + (oc<int>) {return 2;}

oc<int>::oc() {mem = 2 ;}

oc<double> ocd ;
oc<int> oci ;


// *****************************


main () {
  start_test(__FILE__) ;

  check(ocd.foo() == 1) ;
  check((ocd + ocd) == 1) ;
  check(ocd.mem == 1) ;
  
  check(oci.foo() == 2) ;
  check((oci + oci) == 2) ;
  check(oci.mem == 2) ;
  
  end_test() ;
}
