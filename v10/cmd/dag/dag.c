/* DAG - draw a directed graph
 * Gansner North Vo
 */

const char*	Version			=	"\n@(#)Version M-03/31/89\0\n";

#include "draw_dag.h"
#include "dag.h"
#include "parsedag.h"
#include "TrieFA.ins.c"

/* useful constants */
const double	Pi			= 3.1415926537;
const double	IPP			= .01;	// real inches per point of type 
const int		Resolution 	= 72;	// points per virtual inch

/*** globals ***/
Infile			Current_file;
boolean			Uselib	=	true;	// user specified lib with -l option
char*			Lib_path;			// library directory 
int				N;			// number of nodes
int				Extent;		// extent of node and edge array including spares
DAG_node_t**	Node;		// node list
DAG_edge_t**	Edge;		// edge list
user_info_t		User;		// user options from .GS line
options_t		Options;	// user options from graph input (for draw_dag)
char*			Cmd_name;	// from command line
output_lang_t	Output_type;
int				Xmax,Ymax;
Point			Page_size;
Point			Margin = {36,36};

void dumprank(int * rank) {
	if (rank)
		while (*rank >= 0) fprintf(stderr,"%d ",*rank++);
	fprintf(stderr,"\n");
}

void dumpgraph() {
	fprintf(stderr,"global ranksep %d nodesep %d\n",Options.ranksep,Options.nodesep);
	for (int i = 0; i < N; i++) {
		fprintf(stderr,"%s -> ",Node[i]->name);
		for (DAG_edge_t *e = Edge[i]; e; e = e->nextof())
			fprintf(stderr,"%s ",Node[e->node]->name);
		fprintf(stderr,"\n	size %d %d\n",Node[i]->height,Node[i]->width);
	}
	fprintf(stderr,"min rank: "); dumprank(Options.source_nodes);
	fprintf(stderr,"max rank: "); dumprank(Options.sink_nodes);
	for (i = 0; i < Options.n_same_nodes; i++) {
		fprintf(stderr,"rank %d: ",i);
		dumprank(Options.same_nodes[i]);
	}
}

/*
 *	translate draw_dag coordinates to preferred system.
 */

void translate() {
	int node;
	if (!User.rotated) {
		switch(Output_type) {
			case Pic:
			case Cip:
			case Postscript:
			case Graphdraw:
				for (node = 0; node < N; node++) {
					Node[node]->pos.y = Ymax - Node[node]->pos.y;
					for (DAG_edge_t *e = Edge[node]; e; e = e->nextof()) {
						for (int sp = 0; e->splinept[sp].x >= 0; sp++)
							e->splinept[sp].y = Ymax - e->splinept[sp].y;
						e->top.y = Ymax - e->top.y;
						e->bottom.y = Ymax - e->bottom.y;
					}
				}
				break;
		}
	}
	else {
		swap(Xmax,Ymax);
		switch(Output_type) {
			case Pic:
			case Cip:
			case Postscript:
			case Graphdraw:
				for (node = 0; node < N; node++) {
					swap(Node[node]->pos.x,Node[node]->pos.y);
					for (DAG_edge_t *e = Edge[node]; e; e = e->nextof()) {
						for (int sp = 0; e->splinept[sp].x >= 0; sp++)
							swap(e->splinept[sp].x,e->splinept[sp].y);
						swap(e->top.x,e->top.y);
						swap(e->bottom.x,e->bottom.y);
					}
				}
				break;
		}
	}
}

/*
 *  compute Xmax, Ymax of drawing region
 */
void find_drawing_size() {
	int node;
	Xmax = 0; Ymax = 0;

	/* find extent of drawing region */
	for (node = 0; node < N; node++) {
		Xmax = max(Xmax,Node[node]->pos.x + (Node[node]->width + 1) / 2);
		Ymax = max(Ymax,Node[node]->pos.y + (Node[node]->height + 1) / 2);
		for (DAG_edge_t *e = Edge[node]; e; e = e->nextof()) {
			for (int sp = 0; e->splinept[sp].x >= 0; sp++) {
				Xmax = max(Xmax,e->splinept[sp].x);
				Ymax = max(Ymax,e->splinept[sp].y);
			}
		}
	}
}

void make_drawing() {
	if (N <= 0) return;

	draw_dag(N,(node_t**)Node,(edge_t**)Edge,Options);
	find_drawing_size();
	translate();

	switch (Output_type) {
		case Pic:
		case Cip:
			emit_pic();
			break;
		case Postscript:
			emit_ps();
			break;
		case Graphdraw:
			emit_graphdraw();
			break;
	}
}

void global_init(char *cmdname) {
	Node	=	new DAG_node_t*[Init_extent];
	Edge	=	new DAG_edge_t*[Init_extent];
	Extent 	=	Init_extent;
	Cmd_name = cmdname;
	Options = reset_options();
}

void graph_init() {
	N = 0;
	Syntax_error = 0;
	User = user_info_t();
	Default_node = Reset_node;
	Default_edge = Reset_edge;
}

void reclaim_nodes() {
	for (int i = 0; i < N; i++) delete Node[i];
}

void reclaim_edges() {
	DAG_edge_t	*e,*f;
	for (int node = 0; node < N; node++) {
		if (e = Edge[node]) {
			if (e->splinept) delete e->splinept;
			while (e) {
				f = e->nextof();
				delete e;
				e = f;
			}
			Edge[node] = 0;
		}
	}
}

void reclaim_samenodesets() {
	delete Options.source_nodes;
	Options.source_nodes = 0;
	delete Options.sink_nodes;
	Options.sink_nodes = 0;
	for (int i = 0; i < Options.n_same_nodes; i++) {
		delete Options.same_nodes[i];
		Options.same_nodes[i] = 0;
	}
	Options.same_nodes = 0;
	Options.n_same_nodes = 0;
}

void reclaim_storage() {
	reclaim_nodes();
	reclaim_edges();
	reclaim_hashtable();
	reclaim_samenodesets();
	freestrings();
}

main(int argc, char**argv) {
	char **args = argv;
	int nfiles = 0;
	boolean use_stdin = true;

	global_init(argv[0]);
	for (int arg = 1; arg < argc; arg++) {
		if (*argv[arg] == '-') {
			switch(argv[arg][1]) {
				case 'O':
					Options.quick = false;
					break;
				case 'l':
					Uselib = false;
					break;
				case 'v':
					Options.verbose = true;
					break;
				case 'T':	// type of output
					set_output_type(&argv[arg][2]);
					break;
				case 'p':
					set_page_size(&argv[arg][2]);
					break;
			}
		}
		else {nfiles++; use_stdin = false;}
	}
	if (use_stdin) nfiles = 1;

	for (int file = 0; file < nfiles; file++) {
		char buf[BUFSIZ];
		if (!use_stdin) Current_file = nextfile(args);
		if (!Current_file.fp) continue;
		if (Output_type == Pic) printf(".lf 1 %s\n",Current_file.name);
		while (Current_file.gets(buf,sizeof(buf))) {
			if (buf[0] == '.' && buf[1] == 'G' &&((buf[2] == 'D')||(buf[2] == 'R'))) {
				/* they're playing our song */
				graph_init();
				if (buf[2] == 'R') User.set_rotate();
				double userwidth = 0., userheight = 0.;
				char fill[80]; fill[0] = 0;
				sscanf(&buf[3],"%lf %lf %s",&userwidth,&userheight,fill);
				User.set_size(userwidth,userheight);
				if (!strcmp(fill,"fill"))
					set_bounds(userwidth,userheight);
				yyparse();
				reclaim_storage();
			}
			else if ((Output_type == Pic) && buf[0] == '.' && buf[1] == 'l' && buf[2] == 'f') {
				char auxbuf[BUFSIZ];
				int line_number;
				if (sscanf(buf + 3,"%d %s",&line_number,auxbuf) == 2) {
					free(Current_file.name);
					printf(".lf %d %s\n",Current_file.line_number = line_number,
						Current_file.name = strcpy(new char[strlen(auxbuf)+1],auxbuf));
				}
				else
					printf(".lf %d\n",Current_file.line_number = line_number);
			}
			else fputs(buf,stdout);
		}
		if (!use_stdin) {
			free(Current_file.name);
			fclose(Current_file.fp);
		}
	}
}

/*** the lexer ***/

static char tokenbuf[BUFSIZ];
static char lexbuf[BUFSIZ];
static char *lexbufptr;

static char *eatwhitespace(char *p) {
	while (isspace(*p)) p++;
	return p;
}

/* grab drawing-code enclosed in matching curly braces.  strip braces. */
static char *scandesc() {
	char sidebuf[BUFSIZ],*p;
	int left = BUFSIZ - 1;
	int depth = 1;	// eat opening curlybrace

	p = sidebuf;
	while (left) {
		if (!*lexbufptr) {
			lexbufptr = Current_file.gets(lexbuf,sizeof(lexbuf));
			if (!lexbufptr) {
				yyerror("unmatched '{' in drawing information");
				return 0;
			}
		}
		if (*lexbufptr == '}') {
			depth--;
			if (depth == 0) {
				*p = 0;
				lexbufptr++;
				return newstring(sidebuf);
			}
		}
		else if (*lexbufptr == '{') depth++;
		*p++ = *lexbufptr++;
		left--;
	}
	yyerror("overran buffer for drawing info");
	return 0;
}

/* bump p and return pointer to (static) null delimited copy of name token */
char *delimitname(char* &p) {
	char *q = tokenbuf;

	p = eatwhitespace(p);
	if (!*p) return 0;
	while ((*p) && !isspace(*p) && (*p != ';') && (*p != '{') && (*p != '}') 
		&& (*p != ',') && (*p != '\"'))
		*q++ = *p++;
	*q = 0;
	return tokenbuf;
}

/* delimit a quoted string */
char *quotelimit(char* &p) {
	char *q = tokenbuf;

	++p;
	while ((*p) && *p != '\"') {
		if ((*p == '\\') && (*(p + 1) == '\"')) p++;
		*q++ = *p++;
	}
	if (!*p) yyerror("quoted string ran off end of line");
	p++;
	*q = 0;
	return tokenbuf;
}

int squirrelbufinuse,squirrelbufsize;
char *squirrelbuf;
void squirrel(const char*p)
{
	int l = strlen(p);
	if (l > squirrelbufsize - squirrelbufinuse) {
		if (!squirrelbuf) squirrelbuf = (char*)malloc(BUFSIZ);
		else squirrelbuf = (char*)realloc(squirrelbuf,squirrelbufsize += BUFSIZ);
	}
	strcpy(squirrelbuf + squirrelbufinuse,p);
	squirrelbufinuse += l;
}
void unsquirrel()
{
	if (squirrelbuf) fputs(squirrelbuf,stdout);
	squirrelbufinuse = 0;
}

void copyPS()
{
	char buf[BUFSIZ];
	char *p;
	while (p = Current_file.gets(buf,sizeof(buf))) {
		if (p[0] == '.' && p[1] == 'P' && p[2] == 'E') return;
		else squirrel(buf);
	}
	yyerror("unmatched .PS inside .GS");
}

int myyylex()
{
	int rv = myyylex();
	fprintf(stderr,"returning %d\n",rv);
	if (rv == STRING || rv == DESC ) fprintf(stderr,"string val is %s\n",yylval.s);
	return rv;
}

int yylex()
{
	if (lexbufptr) lexbufptr = eatwhitespace(lexbufptr);
	while (!lexbufptr || !*lexbufptr) {
		while ((lexbufptr = Current_file.gets(lexbuf,sizeof(lexbuf)))
		&& (lexbuf[0] == '.')) {
			if (lexbuf[1] == 'P' && lexbuf[2] == 'S') copyPS();
			else {
				if (lexbuf[1] == 'G' && lexbuf[2] == 'E') {lexbufptr = 0;return EOF;}
				else squirrel(lexbuf);	// assume it's a troff control
			}
		}
		if (!lexbufptr) {
			yyerror("unmatched .GS");
			return EOF;
		}
		lexbufptr = eatwhitespace(lexbufptr);
	}

	switch(*lexbufptr) {
		case ',':
		case ';':
			return *lexbufptr++;
		case '\"':
			yylval.s = newstring(quotelimit(lexbufptr));
			return STRING;
		case '}':
			yyerror("unexpected '}'");
			lexbufptr++;
			return ';';
		case '{' :
			lexbufptr++;
			yylval.s = scandesc();
			return DESC;
	}
	char *tokenptr = delimitname(lexbufptr);
	char *p = tokenptr;

	TFA_Init();
	while (*p) {
		TFA_Advance(*p);
		p++;
	}
	int token = TFA_Definition();
	if (token != -1) return token;
	yylval.s =  newstring(tokenptr);
	return STRING;
}

void error_context()
{
	char *p,*q;
	if (!lexbufptr) return;
	fprintf(stderr,"context is: ");
	for (p = lexbufptr - 1; (p > lexbuf) && (!isspace(*p)); p--);
	for (q = lexbuf; q < p; q++) fputc(*q,stderr);
	fputs(" >>> ",stderr);
	for (; q < lexbufptr; q++)fputc(*q,stderr);
	fputs(" <<< ",stderr);
	fputs(lexbufptr,stderr);
}
void yyerror(char*s,char *s1, char* s2, char *s3, char *s4) {
	extern void error_context();

	if (Syntax_error) return;
	Syntax_error = 1;
	fprintf(stderr, "%s: ", Cmd_name);
	fprintf(stderr, s, s1, s2, s3, s4);
	fprintf(stderr, " near line %d, file %s\n",Current_file.line_number,
		Current_file.name);
	error_context();
}
