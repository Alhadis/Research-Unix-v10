/* ahdr.h: definitions for atc game */

/*	#ifdef vax
	#define AIRFILE "/r/jim/atc/airspaces"
	#define STATSFILE "/r/jim/atc/stats"
	#else
	#define AIRFILE "/g/jim/atc.export/airspaces"
	#define STATSFILE "/g/jim/atc.export/stats"
	#endif						*/

#define AIRFILE "/usr/games/lib/atc/airspaces"
#define STATSFILE "/usr/games/lib/atc/stats"


#define LEFT 0          /* left is left margin */
#define BOTTOM 39       /* bottom addressable part of display */

#define VERBOSE 1
#define TERSE 0

#define YES 1
#define NO 0

#define FF 014
#define SI 017
#define FS 034
#define US 037
#define BELL 007
#ifdef ANNARBOR
#define BS 036
#else
#define BS '\b'
#endif

#define BEEP putchar(BELL)

#define RETURN          '\r'
#define MIPAGE          021
#define HOME            013
#define PLPAGE          031
#define MISRCH          005
#define UPARROW         016
#define PLSRCH          022
#define LEFTARROW       010
#define DOWNARROW       012
#define RIGHTARROW      037
#define INSMODE         017

#define TIMEOUT 3       /* he loses command if he waits >3" between chars */

#define PAUSE putchar(0); putchar(0); putchar(0)

#define MINWIDTH 10
#define MAXWIDTH 30
#define MINHEIGHT 10
#define MAXHEIGHT 40

#define MAXPATHS 200    /* max number of paths for flow control */

#define EMAX 20 /* max number of entries into the screen */
#define AMAX 5
#define NMAX 5

#define CMDLEN 10       /* max length command: e.g. ATNW<0> */
#define CDLNAME 10      /* max length of name of clearance directive list */
#define LOCLEN 30       /* max length of location identifier */

#define NALT 12         /* max number altitudes being stored in f_dist */

struct flow /* from and to are stored as the die roll needed in flightplan */
{       int f_from;     /* origin */
	int f_fair;     /* 1 if airport */
	int f_to;       /* destination */
	int f_tair;     /* 1 if airport */
	int f_freq;     /* relative frequency */
	int f_dist[NALT]; /* least dist to travel if it comes in this high */
	int f_cmds;     /* min # of commands to land it */
};

struct pstruct  /* structure for interesting points on the screen */
{       int p_x;        /* coordinates of the entry point */
	int p_y;
	int p_dx;       /* x increment */
	int p_dy;
	int p_inprop;   /* proportion of entries from here */
	int p_outprop;  /* proportion of exits to here */
	char p_sym;     /* symbol on screen */
};

struct cdl      /* clearance directive list (plan) */
{       struct cdl *c_next;
	struct list *c_list;
	char c_name[CDLNAME];
} cdltop;

struct list     /* list of clearances */
{       struct list *l_next;
	int l_x,l_y;          /* place where this takes effect */
	char l_loc[LOCLEN];     /* verbatim location description */
	char l_cmd[CMDLEN];  /* command to execute here */
};

#define PMAX 26 /* for now we'll have exactly 26 planes as in apple game */

struct astruct  /* airplane structure */
{       int a_stime;    /* starting time (after start) */
	int a_active;   /* 1 if flying; 2 if on standby; 0 if off or waiting*/
	int a_start;    /* starting location */
	int a_sair;     /* 1 if starting from airport; else 0 */
	int a_dest;     /* leaving or landing at ... */
	int a_dair;     /* 1 if landing at airport; else 0 */
	int a_x;        /* location */
	int a_y;
	int a_z;        /* altitude */
	int a_dx;       /* direction: -1,0,or 1 */
	int a_dy;
	int a_lastz;    /* climb or descent - used for conflicts */
	int a_nextz;    /* designated altitude */
	char a_turn;    /* turning 'L' or 'R' (0 if no turn) */
	int a_dxnew;    /* new course - x increment */
	int a_dynew;
	int a_type;     /* 0 for prop, 1 for jet */
	int a_fuel;
	int a_hold;     /* 1 if scheduled to hold at navaid, 2 if holding */
	char a_clear;   /* cleared for approach to this location */
	char a_id;
	struct list *a_plan;     /* "clearance directive list" */
	int a_load;     /* sum of active planes at each tick */
	int a_ticks;    /* number of ticks it's on the screen */
	int a_planned;  /* sum of planned planes at each tick */
	int a_dist;     /* distance this plane travelled */
	int a_opt;      /* best distance it could have travelled */
	int a_cmds;     /* min # of commands for this plane's route */
	int a_prev;     /* number of ticks it was in the preview area */
	int a_aprev;    /* number of planes active while in preview */
	int a_pprev;    /* number of planes planned while in preview */
	int a_pplanned; /* whether this was planned or not while in preview */
};

#define PROP 0
#define JET  1

#define PROPFUEL 21*2
#define JETFUEL  15*4

/* reasons for dying */
#define FUEL 0
#define OUTOFTIME 1
#define CRASH 2
#define BOUND 3
#define SYSTEM 6
#define GOAROUND 7

#define UPDATE 15       /* updates happen every 15 seconds */

#define INACTIVE 0      /* possible airplane statuses (plane[i].a_active) */
#define ACTIVE 1
#define WAITING 2       /* waiting for takeoff */
#define DONE 3
#define APPROACHING 4   /* from another sector */
#define LANDING 5

#define FPTHRESH 60     /* amount of time info on incoming planes is up */
#define NEARLY 2        /* number of planes to get in 1st FPTHRESH seconds */

#define FUDGE 20        /* pretend interval is this much longer to make it */
			/* more likely that there will be planes initially */
#define FIRST_TIME 30   /* first time that planes can come on */
#define SAFETY 120      /* if incoming planes are this close, bump alt */

extern char *index();

/*#define OFFSET 0        /* 0-origin */
/*#define OFFSET -1       /* 1-origin */
#define OFFSET -2       /* 2-origin */


