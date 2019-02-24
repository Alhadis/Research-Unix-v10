/* ident "@(#)ctrans:src/tree_dump.h	1.2" */
/* -*- Mode: C -*- Begin tree_dump.h */

#ifndef _DUMP_TREE
#define _DUMP_TREE

#include "tree_walk.h"
#include <iostream.h>
#include "hash.h"

enum dump_tree_verbosity {
    dt_brief = 1,		/* minimal identification */
    dt_normal = 2,	  	/* all data, outpointers Brief (one level) */
    dt_recursive_0 = 3,		/* minimal, but recurse down.
				   (outpointers Recursive_0) */
    dt_recursive_1 = 4,		/* all data, outpointers Recursive_2. */
};

class dcn_arg {
  public:
    int version;
    dump_tree_verbosity verbose;
    tree_fetch_proc fetcher;
    void * fetcher_info;
    ostream* output_stream;
    ostream* error_stream;
    int max_depth;
    Hash * nodes_seen_hash;
    /* end of version 1 */
    int walk_tables;
    /* end of version 2 */
    int stop_at_top;
    dcn_arg () { version = 2; 
		 nodes_seen_hash = 0;
		 fetcher = null_tfp; 
		 output_stream = &cout;
		 error_stream = &cerr; 
		 max_depth = -1;
		 walk_tables = 1;
		 stop_at_top = 0;
	     };
};


extern "C" {
    void display_cfront_node (dcn_arg&, Pnode);
}
#else

typedef enum dump_tree_verbosity {
    dt_brief = 1,		/* minimal identification */
    dt_normal = 2,	  	/* all data, outpointers Brief (one level) */
    dt_recursive_0 = 3,		/* minimal, but recurse down.
				   (outpointers Recursive_0) */
    dt_recursive_1 = 4,		/* all data, outpointers Recursive_2. */
} dump_tree_verbosity;

struct dcn_arg {
    int version;		/* must be 1 for now. */
    dump_tree_verbosity verbose;
    int (*fetcher) ();
    void * fetcher_info;
    void * output_stream;	/* these are c++ streams */
    void * error_stream;
    int max_depth;
    void * nodes_seen_hash;
    int walk_tables;
    int stop_at_top;
};

void display_cfront_node ();
#endif

/* End tree_dump.h */
