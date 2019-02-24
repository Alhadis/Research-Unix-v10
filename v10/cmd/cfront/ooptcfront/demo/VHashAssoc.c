#include "check.h"

// This may look like C code, but it is really -*- C++ -*-
/* 
Copyright (C) 1988 Free Software Foundation
    written by Doug Lea (dl@rocky.oswego.edu)

This file is part of GNU CC.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.  No author or distributor
accepts responsibility to anyone for the consequences of using it
or for whether it serves any particular purpose or works at all,
unless he says so in writing.  Refer to the GNU CC General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute
GNU CC, but only under the conditions described in the
GNU CC General Public License.   A copy of this license is
supposed to have been given to you along with GNU CC so you
can know your rights and responsibilities.  It should be in a
file named COPYING.  Among other things, the copyright notice
and this notice must be preserved on all copies.  
*/

/* GNU hash tables converted to demonstrate PT */

template <class T, class U> class VHashAssocNode
{
public:
  T                     key;
  U                     cont;
  char                  status;
};


template <class T, class U> class VHashAssocTrav ;


template <class T, class U> class VHashAssoc
{
  friend class          VHashAssocTrav<T,U> ;

  VHashAssocNode<T,U> * tab;
  int                   size;
  int                   cnt;

public:
  static unsigned int (*key_hash_function)(T &);
  int  (* key_key_equality_function )(T&, T&);

  unsigned int          key_hash(T& a);
  int                   key_key_eq(T&a, T& b);    

                        VHashAssoc(int sz);
                        VHashAssoc(VHashAssoc& a);
                        ~VHashAssoc();

  VHashAssoc&     operator = (VHashAssoc& a);

  int                   count();
  int                   empty();
  int                   full();
  int                   capacity();

  void                  clear();
  void                  resize(int newsize);

  U&                  operator [] (T& k);
  int                   contains(T& key);
  int                   del(T& key);


  void                  apply(void f(U& v));
  void                  error(const char* msg);
};

template <class T, class U> class VHashAssocTrav
{
  VHashAssoc<T,U>*      h;
  int                   pos;


public:
                        VHashAssocTrav(VHashAssoc<T,U>& l);
                        ~VHashAssocTrav();
  int                   null();
  int                   valid();
                        operator void* ();
  int                   operator ! ();
  void                  advance();
  void                  reset();
  void                  reset(VHashAssoc<T,U>& l);
  T&                    key();
  U&                  get();
};

template <class T, class U>  unsigned int VHashAssoc<T,U>::key_hash(T& a)
{
  return (*VHashAssoc<T,U>::key_hash_function)(a);
}

template <class T, class U>   int VHashAssoc<T,U>::key_key_eq(T&a, T& b)
{
  return (*VHashAssoc<T,U>::key_key_equality_function)(a, b);
}


template <class T, class U>  VHashAssoc<T,U>::~VHashAssoc()
{
  delete [size] tab;
}

template <class T, class U>  VHashAssoc<T,U>::count()
{
  return cnt;
}

template <class T, class U>  VHashAssoc<T,U>::empty()
{
  return cnt == 0;
}

template <class T, class U>  VHashAssoc<T,U>::full()
{
  return cnt == size;
}

template <class T, class U>  VHashAssoc<T,U>::capacity()
{
  return size;
}

template <class T, class U>  VHashAssocTrav<T,U>::VHashAssocTrav(VHashAssoc<T,U>& a)
{
  h = &a;
  reset();
}

template <class T, class U>  void VHashAssocTrav<T,U>::reset(VHashAssoc<T,U>& a)
{
  h = &a;
  reset();
}


template <class T, class U>  VHashAssocTrav<T,U>::~VHashAssocTrav() {}

template <class T, class U>  VHashAssocTrav<T,U>::null()
{
  return pos < 0;
}

template <class T, class U>  VHashAssocTrav<T,U>::valid()
{
  return pos >= 0;
}

template <class T, class U>   VHashAssocTrav<T,U>::operator void* ()
{
  return (pos < 0)? 0 : this;
}

template <class T, class U>  VHashAssocTrav<T,U>::operator ! ()
{
  return (pos < 0);
}


template <class T, class U>  T & VHashAssocTrav<T,U>::key()
{
  if (pos < 0)
    h->error("operation on null traverser");
  return h->tab[pos].key;
}

template <class T, class U> U & VHashAssocTrav<T,U>::get()
{
  if (pos < 0)
    h->error("operation on null traverser");
  return h->tab[pos].cont;
}

enum  {EMPTYCELL, VALIDCELL, DELETEDCELL } ;


// error handling



template <class T, class U>void VHashAssoc<T,U>::error(const char* msg)
{
   // (*VHashAssoc_error_handler)(msg);
}

template <class T, class U>VHashAssoc<T,U>::VHashAssoc(int sz)
{
  tab = new VHashAssocNode<T,U>[size = sz];
  for (int i = 0; i < size; ++i) tab[i].status = EMPTYCELL;
  cnt = 0;
}

template <class T, class U>VHashAssoc<T,U>::VHashAssoc(VHashAssoc<T,U>& a)
{
  tab = new VHashAssocNode<T,U>[size = a.size];
  for (int i = 0; i < size; ++i) tab[i].status = EMPTYCELL;
  cnt = 0;
  for (VHashAssocTrav<T,U> p(a); p; p.advance())
    (*this)[p.key()] = p.get();
}

template <class T, class U> VHashAssoc<T,U>& VHashAssoc<T,U>::operator =
(VHashAssoc<T,U>& a)
{
  if (a.tab != tab)
  {
    clear();
    delete [size] tab;
    tab = new VHashAssocNode<T,U>[size = a.size];
    for (int i = 0; i < size; ++i) tab[i].status = EMPTYCELL;
    cnt = 0;
    for (VHashAssocTrav<T,U> p(a); p; p.advance())
      (*this)[p.key()] = p.get();
  }
  return *this;
}

/* 
 * hashing method: double hash based on high bits of hash fct,
 * followed by linear probe. Can't do too much better if Assoc
 * sizes not constrained to be prime.
*/


static inline doublehashinc(unsigned int h, int s)
{
  return ((h / s) % s) >> 1;
}


template <class T, class U>U& VHashAssoc<T,U>::operator [](T & key)
{
  int bestspot = -1;
  unsigned int hashval = key_hash(key);
  int h = hashval % size;
  for (int i = 0; i <= size; ++i)
  {
    if (tab[h].status == EMPTYCELL)
    {
      if (bestspot < 0) bestspot = h;
      tab[bestspot].key = key;
      tab[bestspot].status = VALIDCELL;
      ++cnt;
      return tab[bestspot].cont;
    }
    else if (tab[h].status == DELETEDCELL)
    {
      if (bestspot < 0) bestspot = h;
    }
    else if (key_key_eq(tab[h].key, key))
      return tab[h].cont;
    if (i == 0)
      h = (h + doublehashinc(hashval, size)) % size;
    else if (++h >= size)
      h -= size;
  }
  error("cannot add to full Assoc table");
  return tab[0].cont ; // suppress warning message
}


template <class T, class U>int VHashAssoc<T,U>::contains(T & key)
{
  unsigned int hashval = key_hash(key);
  int h = hashval % size;
  for (int i = 0; i <= size; ++i)
  {
    if (tab[h].status == EMPTYCELL)
      return 0;
    else if (tab[h].status == VALIDCELL && key_key_eq(tab[h].key, key))
      return 1;
    if (i == 0)
      h = (h + doublehashinc(hashval, size)) % size;
    else if (++h >= size)
      h -= size;
  }
  return 0;
}

template <class T, class U>int VHashAssoc<T,U>::del(T & key)
{
  unsigned int hashval = key_hash(key);
  int h = hashval % size;
  for (int i = 0; i <= size; ++i)
  {
    if (tab[h].status == EMPTYCELL)
      return 0;
    else if (tab[h].status == VALIDCELL && key_key_eq(tab[h].key, key))
    {
      tab[h].status = DELETEDCELL;
      --cnt;
      return 1;
    }
    if (i == 0)
      h = (h + doublehashinc(hashval, size)) % size;
    else if (++h >= size)
      h -= size;
  }
  return 0;
}

/*
template <class T, class U>void VHashAssoc<T,U>::apply(UProcedure f)
{
  for (int i = 0; i < size; ++i)
    if (tab[i].status == VALIDCELL)
      (*f)(tab[i].cont);
}
*/

template <class T, class U>void VHashAssoc<T,U>::clear()
{
  for (int i = 0; i < size; ++i)
    tab[i].status = EMPTYCELL;
  cnt = 0;
}

template <class T, class U>void VHashAssoc<T,U>::resize(int newsize)
{
  if (newsize < cnt)
    error("requested resize too small");
  VHashAssocNode<T,U>* oldtab = tab;
  int oldsize = size;
  tab = new VHashAssocNode<T,U>[size = newsize];
  for (int i = 0; i < size; ++i) 
    tab[i].status = EMPTYCELL;
  cnt = 0;
  for (i = 0; i < oldsize; ++i)
    if (oldtab[i].status == VALIDCELL)
      (*this)[oldtab[i].key] = oldtab[i].cont;
  delete [oldsize] oldtab;
}

template <class T, class U>void VHashAssocTrav<T,U>::reset()
{
  for (pos = 0; pos < h->size; ++pos)
    if (h->tab[pos].status == VALIDCELL)
      return;
  pos = -1;
}

template <class T, class U>void VHashAssocTrav<T,U>::advance()
{
  if (pos < 0)
    return;
  for (pos++; pos < h->size; ++pos)
    if (h->tab[pos].status == VALIDCELL)
      return;
  pos = -1;
}

  
const int size = 100 ;


unsigned int f(int & i) { return i ; }

unsigned int f1(char & i) { return (int)i ; }

int  g(int& k1, int& k2){
  return (k1 == k2) ;
}

int  g1(char & k1, char & k2){
  return (k1 == k2) ;
}




VHashAssoc<int, int> foo(2*size) ;

  
main () {


  start_test(__FILE__) ;

  foo.key_hash_function = f ;
  foo.key_key_equality_function= g ;
  // populate the hash
  for (int i = 0 ; ! foo.full() ; i++)
    foo[i] = i ;

  for (i = 0 ; i < size ; i++) {
    check(foo.contains(i)) ;
    check(foo[i] == i) ;
  }

  foo.resize(size * 4) ;
  
// repeat the tests, resizing should not have impacted it any
  for (i = 0 ; i < size ; i++) {
    check(foo.contains(i)) ;
    check(foo[i] == i) ;
    check (foo.del(i)) ;
    check(!foo.contains(i)) ;
  }
  {
  // try another one
  VHashAssoc<char, char *> foo(2*size) ;
  foo.key_hash_function = f1 ;
  foo.key_key_equality_function= g1 ;
  // populate the hash
  for (char i = 0 ; ! foo.full() ; i++) {
    char *p = new char ;
    *p = i ;
    foo[i] = p  ;
  }

  for (i = 0 ; i < size ; i++) {
    check(foo.contains(i)) ;
    check(*(foo[i]) == i) ;
  }

  foo.resize(size * 4) ;
  
// repeat the tests, resizing should not have impacted it any
  for (i = 0 ; i < size ; i++) {
    check(foo.contains(i)) ;
    check(*(foo[i]) == i) ;
    check (foo.del(i)) ;
    check(!foo.contains(i)) ;
  }
}

  end_test() ;

}
  
