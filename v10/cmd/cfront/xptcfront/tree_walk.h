/* ident "@(#)ctrans:src/tree_walk.h	1.2" */
/* -*- Mode: C -*- Begin include file tree_walk.H */
/*
	$Source: /usr3/lang/benson/work/stripped_cfront/RCS/tree_walk.H,v $ $RCSfile: tree_walk.H,v $
	$Revision: 1.1 $		$Date: 89/11/20 08:51:05 $
	$Author: benson $		$Locker:  $
	$State: Exp $
*/

#ifndef _TREE_WALK
#define _TREE_WALK

#include "cfront.h"
#include "node_classes.h"
#include <iostream.h>
#include "ios_printf.h"
#include "hash.h"

enum tree_node_action {
    tna_continue = 0,		/* plain vanilla */
    tna_stop = 1,		/* don't proceed */
    tna_error = 2,		/* stop entire walk now. */
};

/* procedural interface to indirect via a pointer. */
typedef int (*tree_fetch_proc) (void * info,
				void * pointer,
				unsigned long length,
				int zero_stop,  /* for character strings.*/
				void * target);


const tree_fetch_proc null_tfp = (tree_fetch_proc) 0;

class tree_walk_tree {
  public:
    tree_walk_tree * parent;
    char storage[1];		/* variable amt, specified by caller. */
};

/* The trailing int here controls whether to remember the
   node in the hash table to short-circuit meeting it again */

typedef void (*tree_pre_action)
    (Pnode&, node_class, void *, tree_node_action&, int, Pnode, 
     tree_walk_tree&, int&);


typedef void (*tree_post_action)
    (Pnode&, node_class, void *, tree_node_action&, int, Pnode, 
     tree_walk_tree&);

typedef void (*errorp) (char, const char *);

class tree_walk_control {
  public:
    int version;
    void * callback_info;
    tree_fetch_proc fetcher;
    tree_pre_action action_proc;
    tree_post_action post_action_proc;
    int (*i_error) (int, const char *); /* usually part of cfront */
    int call_i_error;		/* if 0, just use the following */
    ostream * error_stream;
    Hash * nodes_seen_hash;	/* allows this to be global over
				 more than one call to the walker. */
    int resolve_by_name;
    int alloc_stack_bytes;
    /* if on, n_list and s_list are ignored for the very top node. 
       this is for the benefit of the printer. */
    int dont_chase_lists_top;
    tree_walk_control () {
	version = 1;
	callback_info = 0;
	fetcher = null_tfp;
	call_i_error = 0;
	error_stream = &cerr;
	nodes_seen_hash = 0;
	resolve_by_name = 1;	/* the dumper turns this off */
	alloc_stack_bytes = 0;
	post_action_proc = 0;
	dont_chase_lists_top = 0;
    }
};

tree_node_action walk_tree (tree_walk_control& c, Pnode& n);

#endif
