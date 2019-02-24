/* ident "@(#)ctrans:src/hash.c	1.2" */
/*
    $Header: /usr2/odi/libmisc/RCS/hash.C,v 1.11 90/03/22 09:24:07 sam Exp $

    Copyright (c) 1989 by Object Design, Inc., Burlington, Mass.
    All rights reserved.

*/

#include <stdio.h>
#include "hash.h" 
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
