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


/************************ for testing **************************/

#define inline


template <class T> class VStack
{
  int                   size;
  int                   ptr;
  T*                   s;

public:

                        VStack(int sz);
                        ~VStack();

  void                  push(T& item);
  T                   pop();
  int                   pop(T & x);
  T&                  top();
  void                  del_top();

  int                   length();
  int                   empty();
  int                   full();
  int                   capacity();
  void                  resize(int sz);
  void                  clear();
  void                  error(const char* msg);
};


template <class T> inline VStack<T>::VStack(int sz)
{
  s = new T [size = sz];
  ptr = 0;
}

template <class T> inline VStack<T>::~VStack()
{
  delete [size] s;
}



template <class T> inline void VStack<T>::clear()
{
  ptr = 0;
}

template <class T> inline int VStack<T>::capacity()
{
  return size;
}

template <class T> inline int VStack<T>::empty()
{
  return ptr == 0;
}

template <class T> inline int VStack<T>::full()
{
  return ptr == size;
}

template <class T> inline int VStack<T>::length()
{
  return ptr;
}

template <class T> inline void VStack<T>::push(T& item)
{
  if (full()) error("push to full stack.");
  s[ptr++] = item;
}

template <class T> inline T VStack<T>::pop()
{
  if (empty()) error("pop from empty stack.");
  return s[--ptr];
}

template <class T> inline int VStack<T>::pop(T & x)
{
  if (empty()) 
    return 0;
  else
  {
    x = s[--ptr];
    return 1;
  }
}

template <class T> inline void VStack<T>::del_top()
{
  if (empty()) error("del_top from empty stack.");
  --ptr;
}

template <class T> inline T& VStack<T>::top()
{
  if (empty()) error("top from empty stack.");
  return s[ptr-1];
}


template <class T> void VStack<T>::error(const char* msg)
{

}


template <class T> void VStack<T>::resize(int newsz)
{
  if (newsz < ptr)
    error("resize: new size too small");
  T* news = new T [newsz];
  for (int i = 0; i < ptr; ++i)
    news[i] = s[i];
  delete [size] s;
  s = news;
  size = newsz;
}

/*  
extern void default_<T>VStack_error_handler(char*);
extern one_arg_error_handler_t <T>VStack_error_handler;

extern one_arg_error_handler_t 
        set_<T>VStack_error_handler(one_arg_error_handler_t f);
*/


const int stack_size = 10 ;


VStack<int> s1(stack_size) ;



main () {
// VStack<int> s1(stack_size), s2(stack_size), *s3 = new VStack<int>(100) ;
   VStack<int> s1 = VStack<int>(stack_size) ;
  
   int limit = s1.capacity() ;
  start_test(__FILE__) ;

  for (int i = 0 ; i < limit ; i++) {
    s1.push(i) ;
  }

  s1.resize(100) ;
  

  for ( i = limit-1 ; i < 0 ; i--) {
    int v ;
    if (v = s1.top() != i)
      s1.error("failed s1.top") ;
    if (v = s1.pop() != i)
      s1.error("failed s1.pop") ;
  }


   VStack<int *> s2 = VStack<int *>(stack_size) ;
  
   limit = s2.capacity() ;

   for ( i = 0 ; i < limit ; i++) {
     int *p = new int ;

     *p = i ;
     s2.push(p) ;
   }

   s2.resize(100) ;
  

   for ( i = limit-1 ; i < 0 ; i--) {
     int *v ;
     if (*(v = s2.top()) != i)
       s2.error("failed s2.top") ;
     if (*(v = s2.pop()) != i)
       s2.error("failed s2.pop") ;
   }

   end_test() ;
}
     
