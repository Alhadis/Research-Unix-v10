/*ident	"@(#)ctrans:src/main.c	1.6.7.37" */
/***********************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

main.c:

	Initialize global environment
	Read argument line
	Start compilation
	Clean up and exit

**************************************************************************/

#ifdef __cplusplus
#include <stdlib.h>
#endif

#include <ctype.h>
#include "cfront.h"
#include <string.h>
#include "dump_tree.H"
#include <libc.h>
#include <fstream.h>
#include "template.h"
#include <osfcn.h>
#include "hash.h"

int zdebug = 0 ;  // a debugging toggle
char* src_file_name;
char* line_format = "\n# %d \"%s\"\n";

int dump_tree;
dcn_arg dump_tree_arg;
int tree_dump_brief = 0;
int catch_delete_bugs = 0;


char* prog_name = "<<PTcfront 2.00>>";
#ifdef unix
#include <signal.h>

static void core_dump(int = 0)
{
	if (error_count)
		fprintf(stderr,"sorry, cannot recover from previous error\n");
	else {
		extern void print_loc();
		print_loc();
		error('i',"bus error (or something nasty like that)");
	}
	ext(99);
}
#endif

Plist isf_list;
Pstmt st_ilist;
Pstmt st_dlist;
Ptable sti_tbl;
Ptable std_tbl;
Plist stat_mem_list;

int Nfile;// = 1;
int vtbl_opt = -1;	// how to deal with vtbls:
			// -1 static and defined
			// 0 external and supposed to be defined elsewhere
			// 1 external and defined
int debug_opt;
bit limit_renaming = 0;	/* rick, ODI, 3-23-89; used to switch off some
			   cfront renaming. */
bit capture_renaming = 0;/* Rick, ODI; used to switch off cfront renaming 
			    capture. */

int ansi_opt;
int strict_opt;		// disallow features from the anachronism section
			// of the manual
int warning_opt;	// produce more warnings if set

extern simpl_init();
extern typ_init();
extern syn_init();
extern lex_init();
extern error_init();

static char *st_name(char*);	// generates names of static ctor, dtor callers
static Pname def_name;

Hash * dumper_node_hash_table;

void static
clean_dumper_hash_table ()
{
    if(!dumper_node_hash_table) return;
    else {
	HashWalker hi(*dumper_node_hash_table);

	while(hi.valid ()) {
	    struct node& n = *(struct node *) hi.key ();
	    if(n.permanent == 0 || n.permanent == 3) /* flush deleted nodes */
		dumper_node_hash_table->del(hi.key());
	    hi.advance();
	}
    }
}

int syn_count = 0; /* for ease of setting conditional breakpoints
		      to find particular expressions */

void recycle_deleted_storage ();

void run()
/*
	run the appropriate stages
*/
{
	Pname n;
 	// Allocate the canonical templ_compilation instance
         templp = new templ_compilation ;
 

	if(dump_tree) {
	    dumper_node_hash_table =
		dump_tree_arg.nodes_seen_hash = new pointer_hash (10000);
	    dump_tree_arg.verbose = dt_recursive_1;
	}

	while (n=syn()) {
		if (n == Pname(1)) continue;
		if (n->n_list) PERM(n->tp);

//printf( "\nrun: syn(): %s", n->string );
                templp->instantiate_ref_templ() ;

		for (Pname nx, nn=n; nn; nn=nx) {
			Pname rr;

			syn_count ++;

			nx = nn->n_list;
			nn->n_list = 0;


//error('d',"run: dcl(): %s",nn->string);

			rr=nn->dcl(gtbl,EXTERN);

			if(rr == 0 || nn->base==0 || error_count) goto NEXT_DEFINITION;


//error('d',"run: simpl(): %n",nn);

			nn->simpl();

			if (error_count) {
			    if(dump_tree) {
				display_cfront_node (dump_tree_arg, nn);
				*dump_tree_arg.output_stream << "---------------------------\n\n";
			    }
			    goto NEXT_DEFINITION;
			}

//error('d',"run:print(): %n (%k) tp %t",nn,nn->base,nn->tp);

			if (nn->base == TNAME)
				nn->dcl_print(0);
			else {
				Ptype t = nn->tp;
			llxx:
				switch (t->base) {
				case TYPE:
					t = Pbase(t)->b_name->tp; goto llxx;
				case CLASS:
 					if (vtbl_opt==1) n->dcl_print(0);
					break;
				case ENUM:
					Penum(n->tp)->dcl_print(0);
					break;
				case FCT:
//error('d',"fct%n inline %d body %d",nn,Pfct(t)->f_inline,Pfct(t)->body);
					if (Pfct(t)->body==0
					|| (debug_opt==0 && Pfct(t)->f_inline)
					|| Pfct(t)->f_imeasure && Pfct(t)->f_inline==0) break;
//error('d',"rr %n inline %d body %d",rr,Pfct(rr->tp)->f_inline,Pfct(rr->tp)->body);
					rr->dcl_print(0);
					break;
	
				default:
					nn->dcl_print(0);
				}
			}



			if(dump_tree) {
			    display_cfront_node (dump_tree_arg, nn);
			    *dump_tree_arg.output_stream << "---------------------------\n\n";
			}

			if (error_count) goto NEXT_DEFINITION;

			switch (nn->tp->base) {	// clean up
			default:
			{	Pexpr i = nn->n_initializer;
//error('d',"default nn %n i %d",nn,i);
				if (i && i!=Pexpr(1)) {
					DEL(i);
					nn->n_initializer = 0;
					if (def_name==0) def_name = rr;
				}
				break;
			}

			case FCT:
			{	
				Pfct f = Pfct(nn->tp);
				if (f->body
				&& f->f_inline==0
				&& f->f_imeasure==0) {
                                        extern void delete_local();
// error('d', "main() nn: %n local_class: %d %d", nn, local_class, f->local_class );
// modification for local classes defined within inline functions
                                        if ( local_class = f->local_class )  { 
						delete_local();
                                        	local_class = 0;
					}
					DEL(f->body);
					if (def_name==0) def_name = rr;
				}
				break;
			}

			case CLASS:
			{	Pclass cl = Pclass(nn->tp);
				for (Pname px, p=cl->mem_list; p; p=px) {
					px = p->n_list;
					if (p->tp)
			switch (p->tp->base) {
				case FCT:
				{	Pfct f = (Pfct)p->tp;
					if (f->body) {
						if (f->f_inline==0
						&& f->f_imeasure==0) {
                                        	extern void delete_local();
// error('d', "main() member_fct nn: %n local_class: %d", nn, f->local_class );
// modification for local classes defined within inline functions
                                        	if ( local_class = f->local_class )  { 
							delete_local();
                                        		local_class = 0;
						}
						DEL(f->body);
						f->body = 0;
					}
				}
			}
						case CLASS:
						case ENUM:
							break;
						case COBJ:
						case EOBJ:
							DEL(p);
							break;
						default:
							delete p;
						}
					else {
						delete p;
					}
				}
				cl->mem_list = 0;
/* This makes the classdef officially "deleted", which is lunacy. */
/* permanent == 1 is already on, perm == 2 is "expanded" to simpl,
   and perm == 3 is never tested in at&t code. -benson */
#if 0
				cl->permanent = 3;
#endif
				break;
			}
			}
			DEL(nn);
		      NEXT_DEFINITION:
			recycle_deleted_storage ();
		}

		lex_clear();
		if(dump_tree)clean_dumper_hash_table ();

	      }

	templp->end_of_compilation() ;

	switch (no_of_undcl) {
	case 1:		{error('w',"undeclaredF%n called",undcl);}
	case 0:		break;
	default:	{error('w',"%d undeclaredFs called; for example%n",no_of_undcl,undcl);}
	}

	switch (no_of_badcall) {
	case 1:		{error('w',"%n declaredWoutAs calledWAs",badcall);}
	case 0:		break;
	default:	{error('w',"%d Fs declaredWoutAs calledWAs; for example%n",no_of_badcall,badcall);}
	}

	if (error_count) return;

	int i = 1;
	for (Pname m=gtbl->get_mem(i); m; m=gtbl->get_mem(++i)) {
		if (m->base==TNAME
		|| m->n_sto==EXTERN
		|| m->n_stclass == ENUM) continue;

		Ptype t = m->tp;
		if (t == 0) continue;
	ll:
		switch (t->base) {
		case TYPE:	t=Pbase(t)->b_name->tp; goto ll;
		case CLASS:
		case ANON:
		case ENUM:
		case COBJ:
		case OVERLOAD:
		case VEC:	continue;
		case FCT:
				if (Pfct(t)->f_inline || Pfct(t)->body==0) continue;
		}

		if (m->n_addr_taken==0
		&& m->n_used==0
		&& m->tp->tconst()==0
		&& m->n_sto == STATIC)
			error('w',&m->where,"%n defined but not used",m);
	}

        Pname ctor = 0;
	Pname dtor = 0;
	--curloc.line;

	if (st_ilist) {	//	make an "init" function;
			//	it calls all constructors for static objects
		Pname n = new name( st_name("__sti__") );
		Pfct f = new fct(void_type,0,1);
		n->tp = f;
		f->body = new block(st_ilist->where,0,st_ilist);
	//	f->body->s = st_ilist;
		f->body->memtbl = sti_tbl;
		n->n_sto = EXTERN;
		// assignments here are really initializations:
		extern int ignore_const;
		ignore_const++;
		set_linkage("C");
		(void) n->dcl(gtbl,EXTERN);
		set_linkage(0);
		ignore_const--;
		n->simpl();
		n->dcl_print(0);
		ctor = n;
	}

	if (st_dlist) {	//	make a "done" function;
			//	it calls all destructors for static objects
		Pname n = new name( st_name("__std__") );
		Pfct f = new fct(void_type,0,1);
		n->tp = f;
		f->body = new block(st_dlist->where,0,st_dlist);
	//	f->body->s = st_dlist;
		f->body->memtbl = std_tbl;
		n->n_sto = EXTERN;
		set_linkage("C");
		(void) n->dcl(gtbl,EXTERN);
		set_linkage(0);
		n->simpl();
		n->dcl_print(0);
		dtor = n;
	}


#ifdef PATCH
		/*For fast load: make a static "__link" */
	if (ctor || dtor)
	{
	        fprintf(out_file, "static struct __linkl { struct __linkl * next;\n");
#ifdef sun
		fprintf(out_file, "void (*ctor)(); void (*dtor)(); } __link = \n");
#else
		fprintf(out_file, "char (*ctor)(); char (*dtor)(); } __link = \n");
#endif		
	//	fprintf(out_file, "{ (struct __linkl *)0, %s, %s };\n",
	//		ctor_name ? ctor_name : "0",
	//		dtor_name ? dtor_name : "0");
		putstring("{ (struct __linkl *)0, ");
		if (ctor) ctor->print(); else putch('0');
		putch(',');
		if (dtor) dtor->print(); else putch('0');
		putstring("};\n");
	}
#endif

// error( 'd', "run: vlist: %d", vlist );
	 do {
		for (vl* v = vlist; v; v = v->next) v->cl->really_print(v->vt);
		vlist = 0;

		for (Plist l=isf_list; l; l=l->l) {
			Pname n = l->f;
			Pfct f = Pfct(n->tp);
//error('d',"isf %n %t f %d addr %d",n,f,f,n->n_addr_taken);
			if (f->base == OVERLOAD) {
				n = Pgen(f)->fct_list->f;	// first fct
				f = Pfct(n->tp);
			}

			if (debug_opt==0 && n->n_addr_taken) {
				f->f_inline = 0;
				if (n->n_dcl_printed<2) n->dcl_print(0);
			}
		}
	} while (vlist);

	if (strict_opt == 0) { // define static members 
		// patch for SysV VAX -g linkage botch
		for (Plist l=stat_mem_list; l; l=l->l) {
			Pname n = l->f;
//error('d',"stat mem %n %d %d ",n,n->n_initializer,n->n_evaluated);
			if (n->n_initializer==0 && n->n_evaluated==0) {
				Ptype t = n->tp;
				n->n_sto = 0;
				Pname cn = t->is_cl_obj();
				if (cn) (void) t->tsizeof();	// be sure to print class
				if ((cn && Pclass(cn->tp)->has_ctor())
				|| t->is_ref())
					; // force explicit definition
				else
					n->dcl_print(0);
			}
		}
	}

	curloc.putline();
	fprintf(out_file,"\n/* the end */\n");
}

char* afile = "";

int no_of_undcl, no_of_badcall;
Pname undcl, badcall;

extern int list_queries;
ofstream query_list_stream;

void
odi_option(char * ostr)
{
    ostr ++;			/* skip the O */
    switch(*ostr) {
      default:
	fprintf(stderr, "cfront: argument syntax: invalid ODI option.");
	exit(11);
    case 'D':
	catch_delete_bugs = 1;
	/* +DD checks for double deletion */
	ostr ++;
	if(*ostr && *ostr == 'D')catch_delete_bugs ++; 
	
    case 'B':
	tree_dump_brief = 1;
    case 'T':
	dump_tree = 1;
	if(0 == *(ostr + 1) || strcmp (ostr, "T-") == 0)
	    dump_tree_arg.output_stream
		= (ostream *)&cout;
	else {
	    dump_tree_arg.output_stream
		= new ofstream (ostr+1, ios::out);
	    if(dump_tree_arg.output_stream->fail()) {
		perror ("cfront");
		fprintf (stderr, "Failed to open %s\n",
			 ostr + 1);
		exit (11);
	    }
	}
	break;
      case 'o':
	out_file = fopen (ostr+1, "w");
	if(out_file == NULL) {
	    perror ("cfront");
	    fprintf(stderr, "Failed to open %s\n",
		    ostr+1);
	    exit(11);
	}
	break;
      case 'g':
	/* rick, ODI, 3-28-89; used to switch off some
	    cfront renaming. "+gx" causes
	    "limit_renaming" to be set. This turns off
	    the renaming of locals and data members
	    whose names don't clash with a base data
	    member. Additionally, transformed names are
	    captured using the field 
	    name::output_sting. */
	if (*++ostr == 'x') {
	    limit_renaming = 1;
	    capture_renaming = 1;
	} else {
	    fprintf(stderr, "cfront: argument syntax: invalid ODI +g option.");
	    exit(11);
	}
	break;
    }
}


main(int argc, char* argv[])
/*
	read options, initialize, and run
*/
{
	register char * cp;

/* go ahead and dump core. We can learn from it. Benson@odi, 89-05-11 */
#ifndef DEBUGGING
#ifdef unix

#ifdef COMPLETE_SIG_PF
        signal(SIGILL,core_dump);
        signal(SIGIOT,core_dump);
        signal(SIGEMT,core_dump);
        signal(SIGFPE,core_dump);
        signal(SIGBUS,core_dump);
        signal(SIGSEGV,core_dump);
#else
	typedef void (*ST)(int ...);	// trick to circumvent problems with old
	ST sick = ST(&signal);		// (or C) versions <signal.h>
	(*sick)(SIGILL,core_dump);
	(*sick)(SIGIOT,core_dump);
	(*sick)(SIGEMT,core_dump);
	(*sick)(SIGFPE,core_dump);
	(*sick)(SIGBUS,core_dump);
	(*sick)(SIGSEGV,core_dump);

#endif
#endif
#endif /* debugging */

// SUM: not needed
// #ifdef apollo
//	set_sbrk_size(1000000);	// resets free store size
//#else
#ifndef apollo
	(void) malloc(0);	// suppress cashing in V8 malloc
#endif
	error_init();

	for (int i=1; i<argc; ++i) {
		switch (*(cp=argv[i])) {
		case '+':
			while (*++cp) {
				switch(*cp) {
				  case 'O':
				    odi_option (cp);
				    goto xx;
				case 'f':
					src_file_name = cp+1;
					goto xx;
				case 'x':	// read cross compilation table
					if (read_align(afile = cp+1)) {
						fprintf(stderr,"bad size-table (option +x)\n");
						exit(11);
					}
					goto xx;
				case 'e':
					switch (*++cp) {
					case '0':
					case '1': 
						vtbl_opt = *cp-'0';
						break;
					default:
						fprintf(stderr,"bad +e option\n");
						exit(11);
					}
					break;
				case 'd':
					debug_opt = 1;
					break;
				case 'z':
					// Internal debugging toggle
					 zdebug = 1 ;
					 break ;
				case 'w':
					warning_opt = 1;
					break;
				case 'a':
					switch (*++cp) {
					case '0':
					case '1': 
						ansi_opt = *cp-'0';
						break;
					default:
						fprintf(stderr,"bad +a option\n");
						exit(11);
					}
					break;
				case 'p':
					strict_opt = 1;
					break;
				case 'L':
					line_format = "\n#line %d \"%s\"\n";
					break;
				default:
					fprintf(stderr,"%s: unexpected option: +%c ignored\n",
						prog_name,*cp);

				}
			}
		xx:
			break;
		default:
			fprintf(stderr,"%s: bad argument \"%s\"\n",prog_name,cp);
			exit(11);
		}
	}

 	fprintf(out_file,line_format+1,1,src_file_name?src_file_name:""); // strips leading \n
 	fprintf(out_file,"\n/* %s */\n",prog_name);
  	if (src_file_name) fprintf(out_file,"/* < %s > */\n",src_file_name);

//	if (Nspy) {
//		start_time = time(0);
//		print_align(afile);
//	}
	fflush(stderr);
	otbl_init();
	lex_init();
	syn_init();
	typ_init();
	simpl_init();
	scan_started = 1;
	curloc.putline();
	run();
	if(dump_tree) {
	    ostream_printf(*dump_tree_arg.output_stream, "------------ GTBL ------------\n\n");
	    display_cfront_node (dump_tree_arg, ktbl);

	    ostream_printf(*dump_tree_arg.output_stream, "------------ KTBL ------------\n\n");
	    display_cfront_node (dump_tree_arg, ktbl);
	}

//	if (Nspy) {
//		stop_time = time(0);
//		spy(src_file_name);
//	}
	exit( (0<=error_count && error_count<127) ? error_count : 127);
}

char* st_name(char* pref)
/*
	make name "pref|source_file_name|_" or "pref|source_file_name|_"
	where non alphanumeric characters are replaced with '_'
	and add def_name at end to ensure uniqueness
*/
{
	int prefl = strlen(pref);
	int strl = prefl + 2;	// trailing '_' and 0
	if (src_file_name) strl += strlen(src_file_name);
	char* defs;
	int defl;
	if (def_name) {
		defs = def_name->string;
		defl = strlen(defs)+1;	// '_'
	}
	else {
		defs = 0;
		defl = 0;
	}
	char* name = new char[strl+defl];
	strcpy(name,pref);
	if (src_file_name) strcpy(name+prefl,src_file_name);
	name[strl-2] = '_';
	name[strl-1] = 0;
//	char *p = name;
//	while ( *++p ) if (!isalpha(*p) && !isdigit(*p)) *p = '_';
	for (char* p = name; *p; p++) if (!isalpha(*p) && !isdigit(*p)) *p = '_';
	if (defs) {
		strcpy(name+strl-1,defs);	// after the '_'
		name[strl+defl-2] = '_';
		name[strl+defl-1] = 0;
	}
#ifdef DENSE
	void chop(char*);
	chop(name);
#endif

	return name;
}

/* ODI notes -

   tree dumping
   limit_renaming

*/
