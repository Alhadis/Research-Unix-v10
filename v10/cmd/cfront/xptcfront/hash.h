/* ident "@(#)ctrans:src/hash.h	1.2" */
/* Compiler interface to hash tables from odi library. 
	$Source: /usr3/lang/benson/work/stripped_cfront/RCS/hash.h,v $ $RCSfile: hash.h,v $
	$Revision: 1.1 $		$Date: 89/11/20 08:50:36 $
	$Author: benson $		$Locker:  $
	$State: Exp $

    $Header: /usr2/odi/objectstore.src/libos/RCS/hash.H,v 1.4 89/09/26 09:37:31 benson Exp $

    Copyright (c) 1989 by Object Design, Inc., Burlington, Mass.
    All rights reserved.

*/

#ifndef _HASH_H
#define _HASH_H

#include <string.h>

typedef void (*Error_Proc) (const char*) ;

extern void default_Hash_error_handler (const char*) ;
extern Error_Proc Hash_error_handler ;
extern Error_Proc set_Hash_error_handler (Error_Proc f) ;

#ifndef _hash_typedefs
#define _hash_typedefs 1
typedef void (*intProc)(int) ;
#endif

#define DEFAULT_INITIAL_HASH_SIZE 100

struct HashTableEntry
{
  int                   key ;
  int                   cont ;
  char                  status ;
} ;

class HashWalker ;

class Hash
{
  friend class          HashWalker ;

  HashTableEntry*       tab ;
  int                   size ;
  int                   entry_count ;

public:
  unsigned int		(*key_hash_function)(int)  ;
   int            	(*key_key_equality_function) (int, int) ;

  unsigned int          key_hash(int  a) ;
  int                   key_key_eq(int a, int  b);    

                        Hash(int sz) ;
                        Hash(Hash& a) ;
                        ~Hash() ;

  Hash&                 operator= (Hash& a) ;

  int                   count() ;
  int                   empty() ;
  int                   full() ;
  int                   capacity() ;

  void                  clear() ;
  void                  resize(int newsize) ;

  enum insert_action	{ probe, insert, replace };
  void			action (int key, int val, insert_action what,
				int& found, int& old_val);
  int&                  operator [] (int  k) ;
  int                   contains(int  key) ;
  int                   del(int  key) ;

  void                  apply (intProc f) ;
  void                  error(const char* msg) ;
} ;

class HashWalker
{
  Hash*     h ;
  int                   pos ;

public:
                        HashWalker(Hash& l) ;
                        ~HashWalker() ;

  int                   null() ;
  int                   valid() ;
                        operator void* () ;
  int                   operator ! () ;
  void                  advance() ;
  void                  reset() ;
  void                  reset(Hash& l) ;
  const int&            key() ;
  int&                  get() ;
} ;

inline unsigned int Hash::key_hash(int a)
{
#ifdef HASHFUNCTION
  return HASHFUNCTION(a) ;
#else
  return (*key_hash_function)(a) ;
#endif
}

inline int Hash::key_key_eq(int a, int b)
{
#ifdef EQUALITYFUNCTION
  return EQUALITYFUNCTION(a, b) ;
#else
  return (*key_key_equality_function)(a, b) ;
#endif
}


inline Hash::~Hash()
{
  delete [size] tab ;
}

inline int Hash::count()
{
  return entry_count ;
}

inline int Hash::empty()
{
  return entry_count == 0 ;
}

inline int Hash::full()
{
  return entry_count == size ;
}

inline int Hash::capacity()
{
  return size ;
}
inline HashWalker::HashWalker(Hash& a)
{
  h = &a ;
  reset() ;
}

inline void HashWalker::reset(Hash& a)
{
  h = &a ;
  reset() ;
}


inline HashWalker::~HashWalker() {}

inline int HashWalker::null()
{
  return pos < 0 ;
}

inline int HashWalker::valid()
{
  return pos >= 0 ;
}

inline HashWalker::operator void* ()
{
  return (pos < 0)? 0 : this ;
}

inline int HashWalker::operator ! ()
{
  return (pos < 0) ;
}


inline const int& HashWalker::key()
{
  if (pos < 0)
    h->error("operation on null Walker") ;
  return h->tab[pos].key ;
}

inline int& HashWalker::get()
{
  if (pos < 0)
    h->error("operation on null Walker") ;
  return h->tab[pos].cont ;
}

inline int pointer_hasheq (int a, int b)
{
    return a == b;
};

inline unsigned int pointer_hash_fcn (int x)
{
    unsigned X = (unsigned) x;
    return ((X << 16) | (X >> 16)) ^ x;
}

class pointer_hash : public Hash {
  public:
    pointer_hash (int sz = 0) : Hash (sz) {
	key_hash_function = pointer_hash_fcn;
	key_key_equality_function = pointer_hasheq;
    }

    pointer_hash (pointer_hash& h) : Hash (h) {};
};

inline int string_hasheq (int a, int b)
{
    return !strcmp((char *)a, (char *) b);
};

static unsigned int string_hash_fcn (int x)
{
    char * str = (char *)x;
    int l = strlen(str);
	
    if(x <= 4) return str[0];
    else {
	unsigned int * f4 = (unsigned int *) str;
	if (l < 8) return ((*f4 << 16) | (*f4 >> 16)) ^ *f4;
	else {
	    unsigned int * s4 = f4 ++;
	    return ((*f4 << 16) | (*f4 >> 16)) ^ *s4;
	}
    }
};


class string_hash : public Hash {
    public:

    string_hash (int sz = 0) : Hash (sz) {
	key_hash_function = string_hash_fcn;
	key_key_equality_function = string_hasheq;
    };

    string_hash (string_hash& h) : Hash (h) {};
};

#endif
