#include <fio.h>
#include <libc.h>
#include "ipc.h"

#define NULL 0

extern char *optarg;
extern int optind;

int debug;

main(ac, av)
	int ac;
	char *av[];
{
	int c;
	char buf[256];
	char *ns = "/cs/ns";

	while((c = getopt(ac, av, "n:d")) != -1){
		switch(c){
		case 'n':
			ns = optarg;
			break;
		case 'd':
			debug++;
			break;
		}
	}
	if(optind<ac) {
		/*
		 *  do a single command
		 */
		if (command(ns, optind, ac, av)<0)
			exit(1);
	} else {
		/*
		 *  make a connection and just shuttle bytes
		 */
		call(ns);
	}
	_exit(0);
}

/*
 *  do the command from the command line
 */
command(ns, optind, ac, av)
	char *ns;
	int optind, ac;
	char *av[];
{
	char *cp;
	char *types;
	Qset *sp;
	Qtuple *tp;
	char buf[1024];
	int fd;
	extern char *qvalue();
	extern Qset *qset();

	if(strcmp(av[optind], "value")==0 && ac-optind>=3){
		/*
		 *  just return the value
		 */
		types = av[++optind];
		for(++optind; optind<ac; optind++){
			strcat(buf, av[optind]);
			strcat(buf, " ");
		}
		cp = qvalue(types, buf, ns);
		if(cp!=NULL){
			fprint(1, "%s\n", cp);
		} else if(errno!=0){
			fprint(2, "error: %s\n", errstr);
			exit(1);
		}
		return 0;
	} else if(strcmp(av[optind], "values")==0 && ac-optind>=3){
		/*
		 *  return a set of tuples (one tuple per line)
		 */
		types = av[++optind];
		for(++optind; optind<ac; optind++){
			strcat(buf, av[optind]);
			strcat(buf, " ");
		}
		sp = qset(buf, ns);
		if(sp==0 && errno!=0){
			fprint(2, "error: %s\n", errstr);
			exit(1);
		}
		for(; sp; sp = sp->next){
			if(sp->this) for(tp=sp->this; tp; tp=tp->next){
				if((tp->type && strcmp(tp->type, types)==0)
				|| (tp->type==0 && strcmp("name", types)==0))
					fprint(1, "%s\n", tp->value);
			}
		}
		return 0;
	} else if(strcmp(av[optind], "set")==0 && ac-optind>=2){
		/*
		 *  return a set of tuples (one tuple per line)
		 */
		for(++optind; optind<ac; optind++){
			strcat(buf, av[optind]);
			strcat(buf, " ");
		}
		sp = qset(buf, ns);
		if(sp==0 && errno!=0){
			fprint(2, "error: %s\n", errstr);
			exit(1);
		}
		for(; sp; sp = sp->next){
			if(sp->this) for(tp=sp->this; tp; tp=tp->next){
				if(tp->type && *(tp->type))
					fprint(1, "%s,%s ",tp->value,tp->type);
				else
					fprint(1, "%s ", tp->value);
			}
			fprint(1, "\n");
		}
		return 0;
	}

	/*
	 *  something other than set or value, do it the hard way
	 */
	fd = ipcopen(ns, "");
	if(fd<0) {
		fprint(2, "can't reach name server\n");
		exit(1);
	}
	Finit(fd, (char *)0);
	buf[0] = 0;
	for(; optind<ac; optind++){
		strcat(buf, av[optind]);
		strcat(buf, " ");
	}
	fprint(fd, "%s\n", buf);
	return getreply(fd);
}

/*
 *  just send user strings to the name server and vice versa
 */
call(ns)
	char *ns;
{
	int fd;
	char buf[256];
	char *cp;

	fd = ipcopen(ns, "");
	if(fd<0) {
		fprint(2, "can't reach name server\n");
		exit(1);
	}
	Finit(fd, (char *)0);
	fprint(1, ">");
	while((cp=Frdline(0))!=NULL){
		if (*cp != NULL) {
			fprint(fd, "%s\n", cp);
			if(getreply(fd)<0)
				return;
		}
		fprint(1, ">");
	}
}

getreply(fd)
	int fd;
{
	char *cp;

	while((cp=Frdline(fd))!=NULL){
		if(strncmp("OK", cp, 2)==0) {
			continue;
		} else if(strncmp("ILL", cp, 3)==0 || strncmp("BUSY", cp, 4)==0) {
			fprint(1, "%s\n", cp);
			return 0;
		} else if(strncmp("DONE", cp, 4)==0) {
			return 0;
		}
		fprint(1, "%s\n", *cp=='\t'?cp+1:cp);
	}
	return -1;
}
