#include "mgr.h"
#include <ctype.h>

/*
 * routines used by actions 
 */
int doconn(), doexec(), docmd(), dologin();
int auth(), v9auth(), inauth();
int mesgld(), ttyld();
int s5parms(), parms(), asuser(), term();
int gateout(), gateway(), pex(), tcphup();
int readfiles();

/*
 * table of actions
 * one per possible action
 */

typedef struct {
	char *name;	/* as used in the services file */
	int (*func)();	/* function for this action */
	int flag;
} ProtoAction;

#define TAKEARG 1
#define NEEDARG 2
#define IPCACCEPT 4

ProtoAction actions[] = {
	{ "readfiles",	0,	0 },
	{ "login",	doconn,		0 },
	{ "password",	dologin,	0 },
	{ "exec",	doexec,		0 },
	{ "cmd",	docmd,		TAKEARG },	/* arg=command to exec */
	{ "auth",	auth,		IPCACCEPT },
	{ "v9auth",	v9auth,		TAKEARG|IPCACCEPT },
	{ "inauth",	inauth,		TAKEARG|IPCACCEPT },
	{ "mesgld",	mesgld,		0 },
#ifdef PEX
	{ "pex",	pex,		0 },
#endif PEX
	{ "ttyld",	ttyld,		0 },
	{ "tcphup",	tcphup,		0 },
	{ "s5args",	s5parms,	0 },
	{ "args",	parms,		0 },
	{ "term",	term,		TAKEARG },
	{ "user",	asuser,		NEEDARG|IPCACCEPT },	/* arg=user id */
	{ "gateout",	gateout,	NEEDARG|IPCACCEPT }, /* arg=addr prefix */
	{ "gateway",	gateway,	NEEDARG|IPCACCEPT }, /* arg=addr prefix */
	{ NULL }
};

/*
 *  Parse a string for an action.  Actions are of the form `xxx(yyy)'.
 *  `xxx' selects the action and `yyy' is the argument to the action.
 */
Action *
newaction(cp)
	char *cp;
{
	ProtoAction *pap;
	Action *ap = (Action *)malloc(sizeof(Action));
	char *arg;
	char *rp;

	if(ap==NULL) {
		logevent("out of memory parsing action\n");
		return NULL;
	}
	ap->arg = NULL;

	/* find the xxx */
	for(; isspace(*cp); cp++)
		;
	for(arg=cp; *arg && !isspace(*arg) && *arg!='('; arg++)
		;

	/* find the yyy */
	if(*arg=='(') {
		rp = strrchr(arg, ')');
		if (rp == NULL) {
			logevent("missing `)' in action `%s'\n", cp);
			freeaction(ap);
			return NULL;
		}
		*arg++ = '\0';
		for(; isspace(*arg); arg++)
			;
		*rp = '\0';
	} else
		arg = NULL;

	/* look for the action */
	for(pap=actions; pap->name!=NULL; pap++) {
		if(strcmp(pap->name, cp)==0) {
			if(pap->flag&NEEDARG && arg==NULL) {
				logevent("missing arg in action `%s'\n", cp);
				freeaction(ap);
				return NULL;
			}
			if(arg!=NULL && !pap->flag&TAKEARG) {
				logevent("expected no arg in action `%s'\n", cp);
				freeaction(ap);
				return NULL;
			}
			ap->func = pap->func;
			ap->arg = strdup(arg);
			ap->accept = pap->flag&IPCACCEPT;
			return ap;
		}
	}
	logevent("unknown action `%s'\n", cp);
	return NULL;
}

freeaction(ap)
	Action *ap;
{
	if (ap==(Action *)NULL)
		return;
	if (ap->arg!=NULL)
		free(ap->arg);
	free((char *)ap);
}
