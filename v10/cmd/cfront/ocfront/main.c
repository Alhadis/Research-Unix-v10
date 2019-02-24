/*ident	"@(#)ctrans:src/main.c	1.6.7.63" */
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
#include "size.h"
#include "cfront.h"

char* prog_name = "<<AT&T C++ Translator 2.1.0+ 04/01/90>>";
char* src_file_name = "";
char* line_format = "\n# %d \"%s\"\n";

#ifdef unix
#include <signal.h>

static void core_dump(int = 0)
{
	if (error_count)
		fprintf(stderr,"sorry, cannot recover from previous error\n");
	else 
		error('i',"bus error (or something nasty like that)");
	ext(99);
}
#endif

Plist isf_list;
Pstmt st_ilist;
Pstmt st_dlist;
Ptable sti_tbl;
Ptable std_tbl;
Plist stat_mem_list;

int vtbl_opt = -1;	// how to deal with vtbls:
			// -1 static and defined
			// 0 external and supposed to be defined elsewhere
			// 1 external and defined
int debug_opt;
int gplus_opt;		// -g to cc, overrides print optimization 
int ansi_opt;
int strict_opt;		// disallow features from the anachronism section
			// of the manual
int warning_opt;	// produce more warnings if set
bit stmtno = 0;

extern simpl_init();
extern typ_init();
extern syn_init();
extern lex_init();
extern error_init();

char *st_name(char*);	// generates names of static ctor, dtor callers, ptbl_vec
Pname def_name;		// first definition in file
Pname pdef_name;	// used with ptbl_vec if function is first definition

void run()
/*
	run the appropriate stages
*/
{
	Pname n;

	while (n=syn()) {
		if (n == Pname(1)) continue;
		if (n->n_list) PERM(n->tp);

		DB( if(Rdebug>=1) error('d',"run: syn(): '%s'", n->string););

		for (Pname nx, nn=n; nn; nn=nx) {
			Pname rr;
			nx = nn->n_list;
			nn->n_list = 0;

			DB( if(Rdebug>=1) error('d',"run: dcl(): %n",nn););

			if ((rr=nn->dcl(gtbl,EXTERN))==0
			|| nn->base==0
			|| error_count) continue;

			DB( if(Rdebug>=1) error('d',"run: simpl(): %n",nn););

			nn->simpl();

			if (error_count) continue;

			DB( if(Rdebug>=1) error('d',"run:print(): %n (%k) tp %t",nn,nn->base,nn->tp););

			if (nn->base == TNAME)
				nn->dcl_print(0);
			else {
				Ptype t = nn->tp;
			llxx:
				switch (t->base) {
				case TYPE:
					t = Pbase(t)->b_name->tp; goto llxx;
				case CLASS:
 					DB( if(Rdebug>=1) fprintf(stderr," -- class\n"););
					if (vtbl_opt==1 || gplus_opt) n->dcl_print(0);
					break;
				case ENUM:
 					DB( if(Rdebug>=1) fprintf(stderr," -- enum\n"););
					Penum(n->tp)->dcl_print(0);
					break;
				case FCT:
					DB( if(Rdebug>=1) error('d'," -- fct%n inline %d body %d",nn,Pfct(t)->f_inline,Pfct(t)->body););
					if (Pfct(t)->body==0
					|| (debug_opt==0 && Pfct(t)->f_inline && ((n->n_table!=gtbl) || strcmp(nn->string,"main")))
					|| Pfct(t)->f_imeasure && Pfct(t)->f_inline==0) break;
					DB( if(Rdebug>=1) error('d',"rr %n inline %d body %d",rr,Pfct(rr->tp)->f_inline,Pfct(rr->tp)->body););
					rr->dcl_print(0);
					break;
	
				default:
					DB( if(Rdebug>=1) error('d'," -- %n tpbase: %k",nn,nn->tp->base););
					// print class def, if appropriate
					// can't use tsizeof() since this may
					// print an error if class isn't defined
					{ for(Ptype tx = t;;) {
						switch ( tx->base ) {
						case VEC:
						    tx=Pvec(tx)->typ;
						    continue;
						case TYPE: case COBJ:
						    tx=Pbase(tx)->b_name->tp;
						    continue;
						}
						Pclass cl = Pclass(tx);
						if ( cl->base == CLASS
						&&  (cl->defined&(DEFINED|SIMPLIFIED))
						&&   cl->c_body==1 )
							cl->dcl_print(0);
						break;
					  }
					}
					nn->dcl_print(0);
				}
			}

			if (error_count) continue;

			DB( if(Rdebug>=1) error('d',"run: cleanup"););
			switch (nn->tp->base) {	// clean up
			default:
			{	Pexpr i = nn->n_initializer;
				DB( if(Rdebug>=1) error('d',"default nn %n i %d",nn,i););
				if (i && i!=Pexpr(1)) {
					DEL(i);
					nn->n_initializer = 0;
					if (def_name==0 && rr->n_scope != STATIC) { 
						def_name = rr;
						if (pdef_name == 0) ptbl_init(0);
					}
				}
				break;
			}

			case FCT:
			{	
				Pfct f = Pfct(nn->tp);
				DB( if(Rdebug>=1) error('d', "function nn: %n local_class: %d %d", nn, local_class, f->local_class ););
				if (f->body
				&& f->f_inline==0
				&& f->f_imeasure==0) {
                                        extern void delete_local();
                                        if ( local_class = f->local_class )  { 
						delete_local();
                                        	local_class = 0;
					}
					if (f->f_this) f->f_this->n_table = 0;
					DEL(f->body);
					if (def_name==0) { 
						def_name = rr;
						if (pdef_name == 0) ptbl_init(0);
					}
				}
				break;
			}

			case CLASS:
			{	Pclass cl = Pclass(nn->tp);
				for (Pname px, p=cl->mem_list; p; p=px) {
				    px = p->n_list;
				    if (p->tp) {
					switch (p->tp->base) {
					case FCT:
					{	Pfct f = (Pfct)p->tp;
						if (f->body) {
							if (f->f_inline==0
							&& f->f_imeasure==0) {
                                        			if ( local_class = f->local_class ) {
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
				    } else {
					delete p;
				    }
				} // for
				cl->mem_list = 0;
				cl->permanent = 3;
				break;
			}
			}
			DEL(nn);
		}
		lex_clear();
	}

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
		case ENUM:
		case OVERLOAD:	continue;
		case COBJ:
		//case ANON:
		case VEC:	break;
		case FCT:
				if (Pfct(t)->f_inline || Pfct(t)->body==0) continue;
		}

//error('d',&m->where,"%n %d %d %s",m,m->n_addr_taken,m->n_used,m->n_sto==STATIC?"static":"");
//xxx doesn't check anon union members as their use bits are set differently
		if (m->n_addr_taken==0
		&& m->n_used==0
		&& m->n_sto == STATIC) { // (static or anon?)
			for(;;) {
//error('d',&m->where,"  %t tconst(): %d",t,t->tconst());
				if(t->base==TYPE) {
					if(t->tconst()) break;
					t=Pbase(t)->b_name->tp;
				} else if(t->base==VEC) {
					t=Pvec(t)->typ;
				} else {
				    if ( t->tconst() == 0 ) {
					if ( t->base != COBJ )
						error('w',&m->where,"%n defined but not used",m);
					else { Pclass cl;
					    cl = Pclass(Pbase(t)->b_name->tp);
					    if ( cl->has_ctor()==0 )
						if ( cl->csu == ANON )
						 ;//error('w',&m->where,"anonymous union defined but not used");
						else
						    error('w',&m->where,"%n defined but not used",m);
					}
				    } // if const
				    break;
				}
			} // for
		} // if static and not used
	}

        Pname ctor = 0;
	Pname dtor = 0;
	--curloc.line;

	if (st_ilist) {	//	make an "init" function;
			//	it calls all constructors for static objects
		DB( if(Rdebug>=1) error('d',"make sti"); );
		Pname n = new name( st_name("__sti__") );
		Pfct f = new fct(void_type,0,1);
		n->tp = f;
		f->body = new block(st_ilist->where,0,st_ilist);
	//	f->body->s = st_ilist;
		f->body->memtbl = sti_tbl;
		n->n_sto = EXTERN;
		// assignments here are really initializations:
		ignore_const++;
		f->f_linkage = linkage_C;
		f->sign();
		(void) n->dcl(gtbl,EXTERN);
		ignore_const--;
		n->simpl();
		n->dcl_print(0);
		ctor = n;
	}

	if (st_dlist) {	//	make a "done" function;
			//	it calls all destructors for static objects
		DB( if(Rdebug>=1) error('d',"make std"); );
		Pname n = new name( st_name("__std__") );
		Pfct f = new fct(void_type,0,1);
		n->tp = f;
		f->body = new block(st_dlist->where,0,st_dlist);
	//	f->body->s = st_dlist;
		f->body->memtbl = std_tbl;
		n->n_sto = EXTERN;
		f->f_linkage = linkage_C;
		f->sign();
		(void) n->dcl(gtbl,EXTERN);
		n->simpl();
		n->dcl_print(0);
		dtor = n;
	}

#ifdef PATCH
		/*For fast load: make a static "__link" */
	if (ctor || dtor)
	{
		printf("static struct __linkl { struct __linkl * next;\n");
		if ( ansi_opt )
			printf("void (*ctor)(); void (*dtor)(); } __link = \n");
		else
			printf("char (*ctor)(); char (*dtor)(); } __link = \n");
	//	printf("{ (struct __linkl *)0, %s, %s };\n",
	//		ctor_name ? ctor_name : "0",
	//		dtor_name ? dtor_name : "0");
		putstring("{ (struct __linkl *)0, ");
		if (ctor) ctor->print(); else putch('0');
		putch(',');
		if (dtor) dtor->print(); else putch('0');
		putstring("};\n");
	}
#endif

	DB( if(Rdebug>=1) error( 'd', "run: vlist: %d", vlist ); );
	 do {
		for (vl* v = vlist; v; v = v->next) v->cl->really_print(v->vt);
		vlist = 0;

		for (Plist l=isf_list; l; l=l->l) {
			Pname n = l->f;
			Pfct f = Pfct(n->tp);
DB( if(Rdebug>=1)error('d',"isf %n %t f %d addr %d",n,f,f,n->n_addr_taken); );
			if (f->base == OVERLOAD) {
				n = Pgen(f)->fct_list->f;	// first fct
				f = Pfct(n->tp);
			}

			if (debug_opt==0 && 
				n->n_addr_taken) {
				f->f_inline = 0;
				if (n->n_dcl_printed<2) {
					if (warning_opt)
						error('w',"out-of-line copy of %n created",n);
					n->dcl_print(0);
				}
			}
		}
	} while (vlist);

	if (strict_opt == 0) { // define static members 
		// patch for SysV VAX -g linkage botch
		for (Plist l=stat_mem_list; l; l=l->l) {
			Pname n = l->f;
// error('d',"stat mem %n %d %d ",n,n->n_initializer,n->n_evaluated);
			if (n->n_initializer==0 && n->n_evaluated==0) {
				Ptype t = n->tp;
				n->n_sto = 0;
				Pname cn = t->is_cl_obj();
				if (cn==0) cn = cl_obj_vec;
				if (cn) (void) t->tsizeof();	// be sure to print class
				if ((cn && Pclass(cn->tp)->has_ctor())
				|| t->is_ref())
					; // force explicit definition
				else
					n->dcl_print(0);
			}
		}
	}

	i = 1;
	for (Pname nm=ptbl->get_mem(i); nm; nm=ptbl->get_mem(++i))  {
		DB( if(Rdebug>=1) error('d',"ptbl anme %s string2 %s key:%k",nm->string==0?"???":nm->string2,nm->string2==0?"???":nm->string2,nm->n_key); );
		if ( nm->n_key == 0 ) { // ptbl used in file, generate definition
			int str1 = (*src_file_name)?strlen(src_file_name)+2:0; // +2 for __
  			int str2 = strlen(nm->string2) - str1;
  		        char *ps = new char[ str2 + 1 ];
  			strncpy( ps, nm->string2, str2 ); // grab vtbl name
   			ps[str2]='\0';
			ps[2] = 'v';
        		fprintf(out_file,"extern struct __mptr %s[];\n",ps);
        		// fprintf(out_file,"struct __mptr* %s = %s;\n",nm->string2,ps);
			ptbl_add_pair(nm->string2,ps);
			delete ps;
		}
	}

	ptbl_init(1);
	curloc.putline();
	fprintf(out_file,"\n/* the end */\n");
}


#ifdef DBG
int Adebug = 0;
int Ddebug = 0;
int Edebug = 0;
int Ldebug = 0;
int Mdebug = 0;
int Ndebug = 0;
int Pdebug = 0;
int Rdebug = 0;
int Sdebug = 0;
int Tdebug = 0;
//Ydebug == yydebug initialized in y.tab.c
void
process_debug_flags( char* p )
{
	// arg to +Dxxx or debugging comments //@!xxx
	// format of xxx == sequence of any combination of
	//	+flags  --  increment flags (default)
	//	-flags  --  decrement flags
	//	0flags  --  unset flags
	// where flags are single chars
	int incr = 1;
	char c;
	fprintf(stderr,"\n*** processing debugging flags '%s'\n",p);
	while ( c = *p++ ) {
		switch ( c ) {
		case '+': incr =  1; break;
		case '-': incr = -1; break;
		case '0': incr =  0; break;
		case ' ': break;
		// flags...
		case 'A': if(incr==0) Adebug=0; else Adebug+=incr; break;
		case 'D': if(incr==0) Ddebug=0; else Ddebug+=incr; break;
		case 'E': if(incr==0) Edebug=0; else Edebug+=incr; break;
		case 'L': if(incr==0) Ldebug=0; else Ldebug+=incr; break;
		case 'M': if(incr==0) Mdebug=0; else Mdebug+=incr; break;
		case 'N': if(incr==0) Ndebug=0; else Ndebug+=incr; break;
		case 'P': if(incr==0) Pdebug=0; else Pdebug+=incr; break;
		case 'R': if(incr==0) Rdebug=0; else Rdebug+=incr; break;
		case 'S': if(incr==0) Sdebug=0; else Sdebug+=incr; break;
		case 'T': if(incr==0) Tdebug=0; else Tdebug+=incr; break;
		case 'Y': if(incr==0) Ydebug=0; else Ydebug+=incr; break;
		default: error('w',"unknown debugging flag '%c'",c);
		}
	}
}
#endif /*DBG*/

int no_of_undcl, no_of_badcall;
Pname undcl, badcall;

main(int argc, char* argv[])
/*
	read options, initialize, and run
*/
{
	register char * cp;
	char* afile = "";
/*
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
*/

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
				case 'g':
					gplus_opt = 1;
					break;
				case 'D':
#ifdef DBG
					process_debug_flags(++cp);
#else
					error('w',"cfront not compiled for debugging -- +D%s ignored",++cp);
#endif
					goto xx;
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
					fprintf(stderr,"%s: unexpected option: +%c ignored\n",prog_name,*cp);

				}
			}
		xx:
			break;
		default:
			fprintf(stderr,"%s: bad argument \"%s\"\n",prog_name,cp);
			exit(11);
		}
	}


	fprintf(out_file,line_format+1,1,src_file_name); // strips leading \n
	fprintf(out_file,"\n/* %s */\n",prog_name);
	if (*src_file_name) fprintf(out_file,"/* < %s > */\n",src_file_name);

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
	if ((BI_IN_BYTE==0) && (SZ_INT==0) && (SZ_WPTR==0))
		error("no size/alignment values - use +x of provide #ifdef in size.h");
	run();
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
	if (*src_file_name) strl += strlen(src_file_name);
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
	if (*src_file_name) strcpy(name+prefl,src_file_name);
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
