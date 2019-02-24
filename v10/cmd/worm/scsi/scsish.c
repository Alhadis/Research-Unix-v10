#include	<stddef.h>
#include	<stdio.h>
#include	<string.h>
#include	"scsi.h"
#include	"scsish.h"
#include	"tcl.h"
#include	"generic/fns.h"

extern Device genericdev;
static Device *dev = 0;
static void parse(FILE *);
Tcl_Interp *interp;

main()
{
	interp = Tcl_CreateInterp();
	scsi_target(2);
	setdevice(&genericdev);
	set_sony();
	printf("dev=%s, target=%d:\n", dev->name, s_id);
	parse(stdin);
	exit(0);
}

static void
parse(FILE *fp)
{
	char line[1000], *cmd;
	int result, partial;
	static Tcl_CmdBuf buffer;

	buffer = Tcl_CreateCmdBuf();
	partial = 0;
	for(;;){
		clearerr(fp);
		if(!partial){
			fputs("% ", stdout);
			fflush(stdout);
		}
		if(fgets(line, sizeof line, fp) == NULL){
			if(!partial)
				exit(0);
			line[0] = 0;
		}
		cmd = Tcl_AssembleCmd(buffer, line);
		if(cmd == NULL){
			partial = 1;
			continue;
		}
		partial = 0;
		result = Tcl_RecordAndEval(interp, cmd, 0);
		if(result == TCL_OK){
			if(interp->result[0])
				printf("%s\n", interp->result);
		} else {
			if(result == TCL_ERROR)
				printf("Error");
			else
				printf("Error %d", result);
			if(interp->result)
				printf(": %s", interp->result);
			printf("\n");
		}
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

void
setdevice(Device *d)
{
	Function *f;
	static char errbuf[256];

	dev = d;
	ss_extsense = dev->extsense;
	for(f = d->fns; f->name; f++){
		Tcl_CreateCommand(interp, f->name, f->fn, (ClientData)errbuf, NULL);
	}
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
gen_help(ClientData err, Tcl_Interp *it, int argc, char **argv)
{
#pragma ref it
#pragma ref err

	if(dev)
		if(help(dev, argc <= 1? 0:argv[1], (Device *)0))
			return(0);
	help(&genericdev, argc <= 1? 0:argv[1], dev);
	return(TCL_OK);
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
gen_dev(ClientData err, Tcl_Interp *it, int argc, char **argv)
{
	Device **d;

#pragma ref it
#pragma ref err

	if(argc == 1)
		printf("dev=%s\n", dev? dev->name : genericdev.name);
	else if(strcmp(argv[1], "?") == 0){
		printf("available devices:\n");
		for(d = devs; *d; d++)
			printf("\t%s(%s)\n", (*d)->name, (*d)->verbose);
	} else {
		for(d = devs; *d; d++)
			if(strcmp(argv[1], (*d)->name) == 0)
				break;
		if(*d){
			setdevice(&genericdev);
			setdevice(*d);
		} else
			fprintf(stderr, "device '%s' unknown\n", argv[1]);
	}
	return(TCL_OK);
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
	char *a[3];

	a[0] = "dev";
	a[1] = "sony";
	a[2] = 0;
	gen_dev(0L, interp, 2, a);
}
