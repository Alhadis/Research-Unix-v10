#include <stdio.h>
#include "trace.h"
#include "trace.d"

 extern int *globvars;
 extern int nrprocs, nrrefs, nrvars;
 extern struct PROCSTACK **procstack;
 extern struct VARPARS *procpars;

 char *Realloc(), *Emalloc(), *Smalloc();

 struct TEMPLATE **templates;
 int nrtemplates = 0, tbound = 0;

growttable()
{ int nsz = tbound + 32;

	if (nsz == 32)
		templates = (struct TEMPLATE **)
			Emalloc(nsz * sizeof(struct TEMPLATE *));
	else
		templates = (struct TEMPLATE **)
			Realloc(templates, nsz * sizeof(struct TEMPLATE *));
	
	tbound = nsz;

}

struct TEMPLATE *
Tinsert()
{ struct TEMPLATE *try;
  register int i;

	if (nrtemplates >= tbound)
		growttable();

	try = (struct TEMPLATE *)
		Smalloc(sizeof(struct TEMPLATE));
	try->l_vars = (struct LOCVARS **)
		Smalloc(nrprocs * sizeof(struct LOCVARS *));
	try->g_vars = (short *)
		Smalloc(nrvars * sizeof(short));

	for (i = 0; i < nrvars; i++)
		try->g_vars[i] = (short) globvars[i];

	if (nrrefs > 0)
		try->traceback = (struct PROCSTACK **)
			Smalloc(nrprocs * sizeof(struct PROCSTACK *));

	for (i = 0; i < nrprocs; i++)
	{	try->l_vars[i] = (struct LOCVARS *)
			Smalloc(sizeof(struct LOCVARS));

		cpylvars(try->l_vars[i], &(procpars[i]));

		if (nrrefs > 0)
		{	if (procstack[i] == NULL)
			{	try->traceback[i] = NULL;
				continue;
			}
			try->traceback[i] = (struct PROCSTACK *)
				Smalloc(sizeof(struct PROCSTACK));
			cpystacks(try->traceback[i], procstack[i]);
	}	}

	templates[nrtemplates] = try;
	nrtemplates++;

	return try;
}

struct TEMPLATE *
inTtable()
{ register int i;

	for (i = 0; i < nrtemplates; i++)
		if (sametempl(templates[i]))
			return templates[i];

	return Tinsert();
}

sametempl(at)
	struct TEMPLATE *at;
{ register int i;

	for (i = 0; i < nrprocs; i++)
	{	if (cmplvars(at->l_vars[i], &(procpars[i])) == 0 ||
		   (nrrefs > 0 && cmpstacks(at->traceback[i], procstack[i]) == 0))
			return 0;
	}

	for (i = 0; i < nrvars; i++)
		if (at->g_vars[i] != (short) globvars[i])
			return 0;

	return 1;
}
