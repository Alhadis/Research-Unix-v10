#include <stdio.h>
#include "pret.h"

struct {
	char name[MAXNAME];
	char mbox;
	char status;
	int  qind;		/* index!=NONE if q addressed is array */
	int  equa;		/* basename match */
	int  code;
} mesgtable[MANY];

struct QTABLE qtable[NQUEUES];

int msize[NQUEUES];		/* mailbox sizes per queue */

int msgval = BASEVAL;
int nrmesgs = 0;
int nrqs = 0;

int initable[MANY];
int nrinits = 0;		/* number initial messages */

extern anyerror, pid, rid;

newqname(str, mask, lim, qind)
	char *str;
{
	register int i;

	if (rid != NONE)
	{	if ((i = Fparname(str, rid, ISQ, NONE, mask, qind)) != -1)
			return (MANY + i);	/* id of qset + offset */
	}
	for (i = 0; i < nrqs; i++)
		if (strcmp(str, qtable[i].name) == 0)
			break;
	if (i == nrqs)
	{	if (nrqs >= NQUEUES)
			whoops("too many queues");

		qtable[i].owner = (mask == DCL || mask == RFR) ? pid : NONE;
		qtable[i].limit  = (lim == NONE) ? 2 : lim;
		qtable[i].status = mask;
		qtable[i].magic  = 0;
		qtable[i].multiple  = qind;
		strcpy(qtable[nrqs++].name, str);
	} else
	{	if (mask == DCL)
		{	if (qtable[i].status & DCL)
				yyerror("queue redeclared, %s", str);

			if (qtable[i].owner == NONE)
				qtable[i].owner = pid;
			else if (qtable[i].owner != pid && pid != NONE)
				warning("queue read by 2 processes", str);
			qtable[i].limit = lim;
			qtable[i].multiple = qind;
		} else if (mask & RFR)
		{	if (qtable[i].owner == NONE)
				qtable[i].owner = pid;

			else if (qtable[i].owner != pid)
				warning("queue read by 2 processes", str);
		}
		if ((mask & RFR) || (mask & ADR))
		{	if (qind != qtable[i].multiple
			&&  (qind == NONE || qtable[i].multiple == NONE))
				yyerror("queue indexing error, mesg name, %s", str);
		}
		qtable[i].status |= mask;
	}
	return i;
}

addmsg(what, hit, mask, tpp, qind)
	char *what; char mask;
{ register int i, j;
  char str[MAXNAME];

	if (rid != NONE)
	{	if ((i = Fparname(what, rid, ISM, hit - MANY, mask, qind)) != -1)
			return (MANY + i);
		if (hit >= MANY)
		{	yyerror("undeclared message, %s", what);
			return -1;
	}	}

	strcpy(str, qtable[hit].name);
	strcat(str, ":"); strcat(str, what);

	for (j = 0; j < nrmesgs; j++)
	{	if (strcmp(str, mesgtable[j].name) == 0)
			break;
	}
	for (i = j; i < nrmesgs; i++)
		if (strcmp(str, mesgtable[i].name) == 0
		&&  mesgtable[i].qind == qind)
			break;
	if (i == nrmesgs)
	{	if (nrmesgs >= MANY)
			whoops("too many messages");
		if (qtable[hit].multiple != qind
		&&  (qind == NONE || qtable[hit].multiple == NONE))
			yyerror("Queue indexing error, qname, %s", str);

		mesgtable[i].mbox = hit;
		mesgtable[i].qind = qind;
		mesgtable[i].status = mask;
		strcpy(mesgtable[nrmesgs++].name, str);
		mesgtable[i].code = msgval++;
		mesgtable[i].equa = mesgtable[j].code;
	} else
		mesgtable[i].status |= mask;

	if (tpp == INITM)
		initable[nrinits++] = mesgtable[i].code;

	if (strcmp(what, " any") == 0 && mask == SAR)
		qtable[hit].magic = 1;

	return mesgtable[i].code;
}

isoqs()
{ int i;
	for (i = 0; i < nrqs; i++)
	{	if (!(qtable[i].status & DCL))
		printf("warning: queue `%s' undeclared\n", qtable[i].name);
		if (qtable[i].owner == NONE)
		printf("warning: queue `%s' unknown owner\n", qtable[i].name);
		switch (qtable[i].status) {
		case 0:		/* used to check name existence in qsets */
		case DCL:	printf("%s: isolated queue\n", qtable[i].name);
				break;
		case DCL+RFR:	printf("%s: queue not addressed\n", qtable[i].name);
				break;
		case DCL+ADR:	printf("%s: queue is never read\n", qtable[i].name);
		default:	break;
		}
	}
}

silentcheck()
{ int i, j, k, p;
	for (i = 0; i < nrqs; i++)
	{	if (msize[i] == 0)
			continue;
		k = strlen(qtable[i].name) + 1;
		for (j = 0; j < nrmesgs; j++)
		if (mesgtable[j].mbox == i && (p = mesgtable[j].status) != SAR)
		{	switch (p) {
			case RCV: if (qtable[i].multiple) break;
				  printf("queue %s: ", qtable[i].name);
				  printf("mesg '%s' ", &mesgtable[j].name[k]);
				  printf("is received but not sent\n");
				  anyerror++;
				  break;
			case SND: if (qtable[i].magic != 0
				  ||  qtable[i].multiple
				  ||  strcmp(&mesgtable[j].name[k], " any") == 0)
				  	break;
				  printf("queue %s: ", qtable[i].name);
				  printf("mesg '%s' ", &mesgtable[j].name[k]);
				  printf("is sent but not received\n");
				  anyerror++;
				  break;
			case 0:   printf("queue %s: ", qtable[i].name);
				  printf("mesg '%s' ", &mesgtable[j].name[k]);
				  printf("is never used\n");
				  break;
			}
	}	}	
}

checkqs()
{	isoqs();
	silentcheck();
}

numsorts(fd)
	FILE *fd;
{ int i, j;

	for (i = j = 0; i < nrqs; i++)
	{	if (qtable[i].multiple == NONE)
			j++;
		else
			j += qtable[i].multiple;
	}
	fprintf(fd, "%d queues:\n", j);
	numesgs(fd);
	for (i = 0; i < nrqs; i++)
	{	fprintf(fd, "%s\t%d/",
				qtable[i].name, qtable[i].owner);
		fprintf(fd, "%d/%d/%d: ",
				qtable[i].limit, msize[i], qtable[i].multiple);
		for (j = 0; j < nrmesgs; j++)
			if (mesgtable[j].mbox == i)
				fprintf(fd, "%d[%d,%d],", mesgtable[j].code,
					mesgtable[j].qind,
					mesgtable[j].equa);
		putc('\n', fd);
}	}

numinits(fd)
	FILE *fd;
{ int i;
	fprintf(fd, "%d inits:\n", nrinits);
	for (i = 0; i < nrinits; i++)
		fprintf(fd, "%d,", initable[i]);
	if (nrinits > 0)
		putc('\n', fd);
}

numesgs(fd)
	FILE *fd;
{ int i, j;
  char c;

	fprintf(fd, "%d messages, base %d:\n", nrmesgs, BASEVAL);
	for (i = 0; i < nrmesgs; i++)
	{	for (j = 0; (c = mesgtable[i].name[j]) != '\0'; j++)
			if (c == ':')
			{	j++;
				break;
			}
		if (c == '\0')
			j = 0;

		fprintf(fd, "%s ", &mesgtable[i].name[j]);
	}
	if (nrmesgs > 0)
		putc('\n', fd);
}

prepsorts()
{ int i, j;
	for (i = 0; i < nrqs; i++)
	{	msize[i] = 0;
		for (j = 0; j < nrmesgs; j++)
			if (mesgtable[j].mbox == i)
				msize[i]++;
	}
}

listqs()
{	int i, j, k, l, a;

	for (i = 0; i < nrqs; i++)
	{
		if (qtable[i].status & DCL == 0)	/* formal q-parameter */
			continue;

		printf("\t%2d\t%s", i+1, qtable[i].name);
		k = strlen(qtable[i].name) + 1;
		if (qtable[i].multiple != NONE)
		{	printf("[%d], ", qtable[i].multiple);
			a = (qtable[i].multiple>9)?4:3;
		} else
		{	printf(", ");
			a = 0;
		}
		for (j = 10; j > k+a; j--) putchar(' ');
		printf("sort: ");

		for (j = l = 0; j < nrmesgs; j++)
		if (mesgtable[j].mbox == i &&
			strcmp(&mesgtable[j].name[k], " tau") != 0 &&
			strcmp(&mesgtable[j].name[k], " any") != 0)
		{	if (l++ > 0)
				printf(", ");
			printf("%s", &mesgtable[j].name[k]);
			if ((a = mesgtable[j].qind) != NONE)
			{	if (a < -1)
					printf("/*%d", -(a+2));
				else
					printf("/%d", a);
		}	}
		printf("\n");
	}
}
