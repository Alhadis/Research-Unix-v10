/* -*- Mode:C++ -*-
    $Header: /usr2/odi/objectstore.src/libos/RCS/hash.H,v 1.4 89/09/26 09:37:31 benson Exp $

    Copyright (c) 1989 by Object Design, Inc., Burlington, Mass.
    All rights reserved.

*/

#ifndef _Hash_h
#define _Hash_h 1

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

#endif




/*
   $Log:	hash.H,v $
Revision 1.4  89/09/26  09:37:31  benson
Don't refer to hash functions using syntax that suggests staticity.

Revision 1.3  89/09/26  09:34:22  benson
hash functions aren't static anymore. They never had any business
being static, and nothing should depend on it.


Revision 1.2  89/07/11  09:03:46  benson
add a procedure to Hash to circument the idiotic [] interface.

Revision 1.1  89/05/22  13:31:29  cwl
Initial revision

 * Revision 1.1  89/05/17  16:06:08  cwl
 * Initial revision
 * 
 * Revision 1.2  89/05/15  14:55:43  cwl
 * remove static rcs decls
 * 
 * Revision 1.1  89/05/12  14:26:52  cwl
 * Initial revision
 * 

   end_log
*/


