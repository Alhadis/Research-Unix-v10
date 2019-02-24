/*
    $Header: /usr2/odi/libmisc/RCS/hash.C,v 1.11 90/03/22 09:24:07 sam Exp $

    Copyright (c) 1989 by Object Design, Inc., Burlington, Mass.
    All rights reserved.

*/

#include <stdio.h>
    /* att.change ??? #include <objectstore/hash.H> */
#include "hash.H" 
#include <osfcn.h>

#define EMPTY   0
#define VALID   1
#define DELETED 2

void default_Hash_error_handler(const char* msg)
{
  fprintf(stderr, "Fatal Hash error: %s\n", msg) ;
  exit(1) ;
}

Error_Proc Hash_error_handler = default_Hash_error_handler ;

Error_Proc set_Hash_error_handler(Error_Proc f)
{
  Error_Proc old = Hash_error_handler ;
  Hash_error_handler = f ;
  return old ;
}

void Hash::error(const char* msg)
{
  (*Hash_error_handler)(msg) ;
}

Hash::Hash(int sz= DEFAULT_INITIAL_HASH_SIZE)
{
  tab = new HashTableEntry[size = sz] ;
  for (int i = 0; i < size; ++i) tab[i].status = EMPTY ;
  entry_count = 0 ;
}

Hash::Hash(Hash& a)
{
  tab = new HashTableEntry[size = a.size] ;

  key_hash_function = a.key_hash_function ;
  key_key_equality_function = a.key_key_equality_function ;

  for (int i = 0; i < size; ++i) tab[i].status = EMPTY ;
  entry_count = 0 ;
  for (HashWalker p(a); p; p.advance())
    (*this)[p.key()] = p.get() ;
}

Hash& Hash::operator = (Hash& a)
{
  if (a.tab != tab)
  {
    clear() ;
    delete [size] tab ;
    tab = new HashTableEntry[size = a.size] ;
    for (int i = 0; i < size; ++i) tab[i].status = EMPTY ;
    entry_count = 0 ;
    for (HashWalker p(a); p; p.advance())
      (*this)[p.key()] = p.get() ;
  }
  return *this ;
}

/* 
 * hashing method: double hash based on high bits of hash fct,
 * followed by linear probe. Can't do too much better if Assoc
 * sizes not constrained to be prime.
*/


static inline doublehashinc(unsigned int h, int s)
{
  return ((h / s) % s) >> 1 ;
}

// IWBNI we knew whether we were being called as an lvalue or rvalue.
// If the former, then we wouldn't have to scan through the whole
// table just to tell if we should rehash or not.  Sigh.
int& Hash::operator [](int key)
{
  unsigned int hashval = key_hash(key) ;
  while (1)
    {
      int bestspot = -1 ;
      int h = hashval % size ;
      for (int i = 0; i <= size; ++i)
	{
	  if (tab[h].status == EMPTY)
	    {
	      // resize if the hash table is more than 87.5% full
	      if (entry_count > ((size>>1)+(size>>2)+(size>>3)))
		// resize and insert again
		break ;
	      if (bestspot < 0) bestspot = h ;
	      tab[bestspot].key = key ;
	      tab[bestspot].status = VALID ;
	      ++entry_count ;
	      return tab[bestspot].cont ;
	    }
	  else if (tab[h].status == DELETED)
	    {
	      if (bestspot < 0) bestspot = h ;
	    }
	  else if (key_key_eq(tab[h].key, key))
	    return tab[h].cont ;
	  if (i == 0)
	    h = (h + doublehashinc(hashval, size)) % size ;
	  else if (++h >= size)
	    h -= size ;
	}
      resize(size << 1)  ;
    }
}

/* This seems convoluted, but it does whatever you want without
   redundant probing of the hash table. */

void Hash::action (int key, int val, insert_action what,
		   int& found, int& old_val)
{
  unsigned int hashval = key_hash(key) ;
  while (1)
    {
      int bestspot = -1 ;
      int h = hashval % size ;
      for (int i = 0; i <= size; ++i)
	{
	  if (tab[h].status == EMPTY)
	    {
	      // resize if the hash table is more than 87.5% full
	      if (entry_count > ((size>>1)+(size>>2)+(size>>3)))
		// resize and insert again
		break ;
	      if (bestspot < 0) bestspot = h ;
	      found = 0;
	      if(what != probe) {
		tab[bestspot].key = key ;
		tab[bestspot].status = VALID ;
		++entry_count ;
		tab[bestspot].cont = val;
	      } 
	      return;
	    }
	  else if (tab[h].status == DELETED)
	    {
	      if (bestspot < 0) bestspot = h ;
	    }
	  else if (key_key_eq(tab[h].key, key)) {
	    found = 1;
	    old_val = tab[h].cont;
	    if(what == replace) 
	      tab[h].cont = val;
	    return;
	  }
	  if (i == 0)
	    h = (h + doublehashinc(hashval, size)) % size ;
	  else if (++h >= size)
	    h -= size ;
	}
      resize(size << 1)  ;
    }
}

int Hash::contains(int key)
{
  unsigned int hashval = key_hash(key) ;
  int h = hashval % size ;
  for (int i = 0; i <= size; ++i)
  {
    if (tab[h].status == EMPTY)
      return 0 ;
    else if (tab[h].status == VALID && key_key_eq(tab[h].key, key))
      return 1 ;
    if (i == 0)
      h = (h + doublehashinc(hashval, size)) % size ;
    else if (++h >= size)
      h -= size ;
  }
  return 0 ;
}

int Hash::del(int key)
{
  unsigned int hashval = key_hash(key) ;
  int h = hashval % size ;
  for (int i = 0; i <= size; ++i)
  {
    if (tab[h].status == EMPTY)
      return 0 ;
    else if (tab[h].status == VALID && key_key_eq(tab[h].key, key))
    {
      tab[h].status = DELETED ;
      --entry_count ;
      return 1 ;
    }
    if (i == 0)
      h = (h + doublehashinc(hashval, size)) % size ;
    else if (++h >= size)
      h -= size ;
  }
  return 0 ;
}

void Hash::apply(intProc f)
{
  for (int i = 0; i < size; ++i)
    if (tab[i].status == VALID)
      (*f)(tab[i].cont) ;
}

void Hash::clear()
{
  for (int i = 0; i < size; ++i)
    tab[i].status = EMPTY ;
  entry_count = 0 ;
}

void Hash::resize(int newsize)
{
  if (newsize < entry_count)
    error("requested resize too small") ;
  HashTableEntry* oldtab = tab ;
  int oldsize = size ;
  tab = new HashTableEntry[size = newsize] ;
  for (int i = 0; i < size; ++i) 
    tab[i].status = EMPTY ;
  entry_count = 0 ;
  for (i = 0; i < oldsize; ++i)
    if (oldtab[i].status == VALID)
      (*this)[oldtab[i].key] = oldtab[i].cont ;
  delete [oldsize] oldtab ;
}

void HashWalker::reset()
{
  for (pos = 0; pos < h->size; ++pos)
    if (h->tab[pos].status == VALID)
      return ;
  pos = -1 ;
}

void HashWalker::advance()
{
  if (pos < 0)
    return ;
  for (pos++; pos < h->size; ++pos)
    if (h->tab[pos].status == VALID)
      return ;
  pos = -1 ;
}
/*
unsigned int foo(int bar) {return bar;}
int baz(int a, int b) {return a == b;}

main()
{
  Hash vh(10)  ;
  HashWalker vt(vh)  ;
  int i  ;

  vh.key_hash_function = &foo  ;
  vh.key_key_equality_function = baz  ;
  printf("Capacity=%d \n", vh.capacity()) ;
  for (i=0; i<500; i+= 5)
    {
      vh[i] = i * i  ;
    }
  vt.reset() ;
  while (vt.valid())
    {
      printf("key=%d, data=%d\t", vt.key(), vt.get());
      vt.advance()  ;
    }
  for (i=0; i<500; i+= 10)
    {
      vh.del (i) ;
      printf("After delete: %d\n", vh[i]);
    }
  printf("\n-----------------\n") ;
  vt.reset() ;
  while (vt.valid())
    {
      printf("key=%d, data=%d\t", vt.key(), vt.get());
      vt.advance()  ;
    }
}

*/

const static char rcsinfo[]= "$Header";
const static char copyright[]="Copyright (c) 1989 by Object Design, Inc., Burlington, Mass.";


/*
   $Log:	hash.C,v $
Revision 1.11  90/03/22  09:24:07  sam
1) Modified the hash tables to resize themselves when they are 87.5% 
full rather than when they overflowed as before. The previous strategy
resulted in ~O(n) probes immediately before the resize.

2) Bug fix: When a hash table was resized, it held on to the location 
of the deleted entry(in bestspot), and used it after the resizing to 
insert a new entry, although the deleted entry had been moved as 
a result of the resizing.

Revision 1.10  90/01/11  13:45:53  dlw
Use stdio instead of AT&T streams.
,

Revision 1.9  90/01/11  13:18:10  benson
include paths.

Revision 1.8  90/01/10  13:05:15  cwl
*** empty log message ***

Revision 1.7  90/01/09  14:59:37  cwl
remove lines which set size prior to doing a resize

Revision 1.6  89/09/27  11:53:46  sam
the copy constructor for Hash needed to initialize the now non-static members
key_hash_function and key_key_equality_function, so that the members of the
argument hash can be copied over.

Revision 1.5  89/08/15  15:17:41  cwl
fix resizing problem

Revision 1.4  89/08/15  14:54:31  root
*** empty log message ***

Revision 1.3  89/07/11  09:04:32  benson
add Hash::action which avoids the pitfalls and inefficiencies
of [] and contains.

Revision 1.2  89/06/28  11:42:04  benson
declare exit by including osfcn.h

Revision 1.1  89/05/22  13:26:17  cwl
Initial revision

Revision 1.1  89/05/12  14:26:40  cwl
Initial revision


   end_log
*/


