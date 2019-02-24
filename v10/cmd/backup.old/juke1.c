#include	<fio.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	"backup.h"
#include	<ipc.h>
#include	<sys/filio.h>
#include	<errno.h>
#include	"juke.h"

/*
	client code
*/
Client *clients = 0;

diskdone(d)
	Disk *d;
{
	Client *c = d->c;
	Disk *a;

	if(d == c->disks)
		c->disks = d->next;
	else {
		for(a = c->disks; a->next != d; a = a->next)
			;
		a->next = d->next;
	}
	diskfree(d);
logprint("donedisk %s(client=%d)\n", d->name, c);
	c->busy = 0;
	if(c->disks == 0)
		clientdone(c);
}

diskfree(d)
	Disk *d;
{
}

clientdone(c)
	Client *c;
{
	Client *cp;

	if(c == clients)
		clients = c->next;
	else {
		for(cp = clients; cp->next != c; cp = cp->next)
			;
		cp->next = c->next;
	}
logprint("freed client %d\n", c);
}

typedef struct Ent
{
	int vol, file;
} Ent;
static Ent ents[100000];
typedef struct Volmap
{
	char *vname;
	int vol;
} Volmap;
static Volmap vms[500];
static Volmap *nv;

static
gobble(iname, e)
	char *iname;
	Ent *e;
{
	register char *p;

	for(p = strchr(iname, 0); --p >= iname;)
		if((*p < '0') || (*p > '9'))
			break;
	if((p < iname) || (*p != '/'))
		return(0);
	e->file = atoi(p+1);
	for(p--; p >= iname; p--)
		if((*p < '0') || (*p > '9'))
			break;
	if((p < iname) || (*p != 'v'))
		return(0);
	e->vol = atoi(p+1);
	return(1);
}

cmp(a, b)
	register Ent *a, *b;
{
	register k;

	if(k = a->vol - b->vol)
		return(k);
	return(a->file - b->file);
}

newclient(fd)
{
	char *b;
	int lim;
	register Client *c;
	register Disk *d;
	register Ent *ne, *de, *e;
	int *p;

	Finit(fd, (char *)0);
	ne = ents;
	c = (Client *)malloc(sizeof(Client));
	c->fd = fd;
	c->busy = 0;
	c->disks = 0;
	c->next = clients;
	clients = c;
	if(b = Frdline(fd))
		c->userinfo = strdup(b);
	else
		c->userinfo = 0;
	while(b = Frdline(fd)){
		if(strcmp(b, "bye") == 0) break;
		if(gobble(b, ne))
			ne++;
	}
	qsort(ents, ne-ents, sizeof(Ent), cmp);
	for(de = ents; de < ne;){
		d = (Disk *)malloc(sizeof(Disk));
		d->busy = 0;
		d->name = vms[lim = vmap(de->vol)].vname;
		d->c = c;
		lim = vms[lim+1].vol;
		for(e = de; (e < ne) && (e->vol < lim); e++)
			;
		d->n = (e-de);
		d->files = (int *)malloc(d->n*2*sizeof(int));
		for(p = d->files; de < e; de++){
			*p++ = de->vol;
			*p++ = de->file;
		}
		d->next = c->disks;
		d->snext = 0;
		c->disks = d;
	}
logprint("newclient %d (%s, disks=%d)\n", c, c->userinfo, c->disks);
	queueclient(c);
}

volmapinit()
{
	int fd;
	char buf[256], buf1[256], *b, *p;

	backupdir(buf1);
	sprint(buf, "%s/%s", buf1, VOLMAP);
	fd = open(buf, 0);
	if(fd < 0){
		write(0, "Error", 1);
		logprint("can't read volmap %s\n", buf);
		exit(1);
	}
	Finit(fd, (char *)0);
	nv = vms;
	while(b = Frdline(fd)){
		if((p = strchr(b, '\t')) == 0){
			write(0, "Error", 1);
			logprint("bad volmap line %s\n", b);
			exit(1);
		}
		*p++ = 0;
		nv->vol = atoi(p);
		nv->vname = strdup(b);
		nv++;
	}
	Fclose(fd);
	nv->vol = 999999;
}

vmap(vol)
{
	register Volmap *v;

	for(v = vms; v->vol <= vol; v++)
		;
	return(v-1-vms);
}
