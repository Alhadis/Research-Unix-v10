#include	"backup.h"
#include	"fail.h"
#include	<fio.h>
#include	<string.h>

Volmap vol[500];
int nvol = 0;

failinit(s)
	char *s;
{
	int fd;
	char *e, *f;
	char volmap[256];

	sprint(volmap, "%s/%s", s, VOLMAP);
	if((fd = open(volmap, 0)) < 0){
		perror(volmap);
		strcpy(vol[0].name, "??");
		vol[0].vol = 0;
		vol[0].fail = 0;
		nvol = 1;
	} else {
		Finit(fd, (char *)0);
		while(e = Frdline(fd)){
			f = strchr(e, '\t');
			*f++ = 0;
			vol[nvol].fail = 0;
			strcpy(vol[nvol].name, e);
			vol[nvol++].vol = atoi(f);
		}
		close(fd);
	}
}
