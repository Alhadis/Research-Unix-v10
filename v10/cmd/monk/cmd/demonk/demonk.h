#define	YES		1	/*  value for true                    */
#define	NO		0	/*  value for false                   */
#define	MAXCOMMAND	512	/*  max # chars in monk command       */
#define MAXC		20	/*  max # white space chars in command*/
#define	MAXMAKE		20	/*  max # chars in make type          */
#define	MAXBEGINS	512	/*  max # nested begins               */
#define	MAXPATH		512	/*  max # chars in file specification */
#define	MAXFILES	20	/*  max # files open simultaneously   */
#define BUFSIZE		100	/*  max # chars in read buffer        */
#define NOTCOMMAND	-1	/*value to indicate chars not a monk command */

#define USAGE		"demonk [-b] [-i] [file...]"

#define DB_PATH		"/usr/lib/monk/db"  /* temporary path: see rd.h */

#define OPTIONS		"ibd:"



struct commnds {
	char	*cname;
	int	ckey;
};

struct commnds todo[] = {
/* the following commands are handled separately without going thru this table:
     associate:   the "name" of the associate is entered into the hash tables
     environment: the "name" of the environment is entered into the hash tables
     make:        the "name" of the doctype is checked for validity and saved
                    in doctype;
     insert:      the contents of specified file are inserted and demonked unless
                    -i option specified, in which case the command is just removed
     source:      the contents of specified file are inserted as is unless                            -i option specified, in which case the command is just removed
     begin:       command specified is followed
     end:         command specified is followed                                 */
/* the following commands (ckey=0) are to be zapped in total along with contents
   of delimiter pair following                                                  */
	{"begin",0},
	{"end",0},
	{"comment",0},
	{"macro",0},
	{"reference",0},
	{"save",0},
	{"remember",0},
	{"savepage",0},
	{"ipa",0},
	{"environment",0},
	{"source",0},
	{"insert",0},
/* the following commands (ckey=1) are to be zapped in total along with contents
   of delimiter pair following AND a space added to the text in its place     
   IF the command NOT preceeded or followed by a newline                        */
	{"sp",1},
	{"p",1},
	{"blank_space",1},
	{"titlebox",1},
	{"reference_placement",1},
	{"signature",1},
	{"att",1},
/* the following commands (ckey=2) are to be zapped with delimiters but contents
   of delimiter pair is to be saved; if no contents (delimiter is white space)
   a space will be inserted in place of the removed command       */
	{"unnumbered_section",2},
	{"unnumbered_subsection",2},
	{"unnumbered_subsubsection",2},
	{"numbered_section",2},
	{"numbered_subsection",2},
	{"numbered_subsubsection",2},
	{"subsection",2},
	{"section",2},
	{"subsubsection",2},
	{"abstract",2},
	{"title",2},
/* the following command (ckey=3) is to be zapped in total along with contents
   of delimiter pair following taking into account that there may be embedded
   identical delimiter pairs in the contents of the original delimiter pair     */
	{"attribute",3},
	{"associate",3},
/* the following commands (ckey=4) must be handled individually             */
	{"table",4},
	{"equation",4},
	{"e",4},
	{"picture",4},
	{"ped",4},
	{"ideal",4},
	{"graph",4},
	{"bargraph",4},
/* the following commands (ckey=5) are to be zapped with delimiters but contents
   of delimiter pair is to be saved and a space inserted before contents        */
	{"notation",5},
	{"footnote",5},
	{"dagnote",5},
	{"starnote",5},
/* all other commands fall into 2 categories: associates and non-associates
   associates (ckey=0) are to be zapped in total along with contents of delimiter
      pair following;
   non associates (ckey=6) are to be zapped with delimiters but contents 
      of delimiter pair saved                                                   */
/* this marks the end of the commands list                                      */
	{NULL,6}
};


struct preprocs {
	char	*monkname;
	char	*macrostart;
	char	*macroend;
};

struct preprocs pp_names[] = {
	{"table",".TS",".TE"},
	{"equation",".EQ",".EN"},
	{"picture",".PS",".PE"},
	{"ped",".GS",".GE"},
	{"ideal",".IS",".IE"},
	{"graph",".G1",".G2"},
	{"bargraph",".B1",".B0"},
	{NULL,NULL,NULL}
};

struct delims {
	int left;
	int right;
};

struct delims delimiters[] = {
	{ '(', ')' },
	{ '[', ']' },
	{ '<', '>' },
	{ '{', '}' },
	{ '\"','\"'},
	{ '\'','\''},
	{ '\`','\''}
};


char	*make_list[] = {
	"acm",
	"centerpb",
	"cspress",
	"form1",
	"global",
	"im",
	"kluwer",
	"letter",
	"memo",
	"model",
	"research",
	"rp",
	"song",
	"tc",
	"tm",
	NULL
};


struct file_info {
	char	*name;
	int	line_count;
};
