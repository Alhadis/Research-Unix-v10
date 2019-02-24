#include	<stddef.h>
#include	<stdio.h>
#include	<string.h>
#include	"scsi.h"
#include	"scsish.h"

extern Device genericdev;
static Device *dev = 0;
static Function *function(char *, Device **);
static void parse(FILE *);

main()
{
	setbuf(stdout, (char *)0);
	scsi_target(2);
	set_sony();
	printf("dev=%s, target=%d:\n", dev? dev->name:genericdev.name, s_id);
	parse(stdin);
	exit(0);
}

static void
parse(FILE *fp)
{
	int i, n;
	char *param;
	char buf[4096];
	char *ptrs[100], *cargs[20];
	int iargs[20];
	int nc, ni;
	Function *fn;
	Device *thatdev;
	char err[512];

	for(;;){
		printf("> ");
		fflush(stdout);
		if(fgets(buf, sizeof buf, fp) == NULL)
			break;
		if(param = strchr(buf, '\n'))
			*param = 0;
		n = getmfields(buf, ptrs, sizeof ptrs/sizeof ptrs[0]);
		if(n < 1)
			continue;
		if((fn = function(ptrs[0], &thatdev)) == 0){
			fprintf(stderr, "can't find cmd '%s'\n", ptrs[0]);
			continue;
		}
		ni = nc = 0;
		param = fn->param;
		for(i = 1; i < n; i++){
			switch(*param++)
			{
			case 'I':
				iargs[ni++] = atoi(ptrs[i]);
				break;
			case 'L':
				iargs[ni++] = atoi(ptrs[i]);
				if((iargs[ni-1] < 0) || (iargs[ni-1] > 7)){
					fprintf(stderr, "%s: lun %d out of range\n", ptrs[0], iargs[ni-1]);
					continue;
				}
				break;
			case 'S':
				cargs[nc++] = ptrs[i];
				break;
			default:
				break;
			}
			if(*param == '?')
				param++;
		}
		while(param[0] && param[1] && (param[1] == '?'))
			param += 2;
		if((i == n) != (*param == 0)){
			printf("param mismatch: %s: i=%d/n=%d param='%s'\n",
				ptrs[0], i, n, param);
			printf("device %s: %s\n", thatdev->name, fn->help);
			continue;
		}
		if((*fn->fn)(ni, iargs, nc, cargs, err))
			fprintf(stderr, "error in '%s': %s\n", fn->name, err);
	}
}

static Function *
flook(Function *f, char *name)
{
	for(; f->help; f++)
		if(strncmp(f->name, name, strlen(f->name)) == 0)
			return(f);
	return(0);
}

static Function *
function(char *name, Device **devptr)
{
	Function *f = 0;

	if(dev && dev->fns && (f = flook(dev->fns, name)))
		*devptr = dev;
	else if(f = flook(genericdev.fns, name))
		*devptr = &genericdev;
	return(f);
}

void
setdevice(Device *d)
{
	dev = d;
	ss_extsense = dev->extsense;
}

static
help(Device *d, char *cmd, Device *prec)
{
	Function *f;
	Function *base;

	base = (prec && prec->fns)? prec->fns:0;
	if(cmd == 0){
		printf("device %s(%s):\n", d->name, d->verbose);
		if(f = d->fns)
			while(f->name){
				if((base == 0) || (flook(base, f->name) == 0))
					printf("\t%s\n", f->help);
				f++;
			}
		return(0);
	} else {
		if(f = d->fns)
			while(f->name)
				if(strcmp(f->name, cmd) == 0){
					printf("(%s) %s\n", d->name, f->help);
					return(1);
				} else
					f++;
		return(0);
	}
}

int
gen_help(int niargs, int *iargs, int ncargs, char **cargs)
{
#pragma ref niargs
#pragma ref iargs

	if(dev)
		if(help(dev, ncargs == 0? 0:cargs[0], (Device *)0))
			return(0);
	help(&genericdev, ncargs == 0? 0:cargs[0], dev);
	return(0);
}

extern Device sonydev;
extern Device wrendev;
static Device *devs[] = {
	&genericdev,
	&sonydev,
	&wrendev,
	0
};

int
gen_dev(int niargs, int *iargs, int ncargs, char **cargs)
{
	Device **d;

#pragma ref niargs
#pragma ref iargs

	if(ncargs == 0)
		printf("dev=%s\n", dev? dev->name : genericdev.name);
	else if(strcmp(cargs[0], "?") == 0){
		printf("available devices:\n");
		for(d = devs; *d; d++)
			printf("\t%s(%s)\n", (*d)->name, (*d)->verbose);
	} else {
		for(d = devs; *d; d++)
			if(strcmp(cargs[0], (*d)->name) == 0)
				break;
		if(*d)
			setdevice(*d);
		else
			fprintf(stderr, "device '%s' unknown\n", cargs[0]);
	}
	return(0);
}

void
scsi_target(int n)
{
	if((n < 0) || (n >= 8))
		fprintf(stderr, "%d is an invalid target\n", n);
	else
		s_id = n;
}

set_sony()
{
	int iargs[1];
	char *cargs[1];

	cargs[0] = "sony";
	gen_dev(0, iargs, 1, cargs);
}
