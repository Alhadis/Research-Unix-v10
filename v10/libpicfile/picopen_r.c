#include <picfile.h>
#include <stdio.h>
#include <libc.h>
#define	NLINE	1024		/* maximum length of header line */
PWR_error(f, buf)
PICFILE *f;
char *buf;
{
	return 0;
}
PICFILE *picopen_r(file)
char *file;
{
	char line[NLINE+1];
	register char *lp;
	char *nchan, *window, *command;
	int x0, y0, x1, y1, i, n;
	PICFILE *p=(PICFILE *)malloc(sizeof(PICFILE));
	if(p==0){
		PIC_error="Can't allocate PICFILE";
		return 0;
	}
	p->flags=PIC_NOCLOSE|PIC_INPUT;
	p->argc=-1;
	p->cmap=0;
	if(strcmp(file, "IN")==0)
		p->fd=0;
	else{
		p->fd=open(file, 0);
		if(p->fd<0){
		Error:
			if(!(p->flags&PIC_NOCLOSE)) close(p->fd);
			if(p->argc>=0){
				for(i=0;i!=p->argc;i++) free(p->argv[i]);
				free((char *)p->argv);
			}
			if(p->cmap) free(p->cmap);
			free((char *)p);
			return 0;
		}
		p->flags=PIC_INPUT;
	}
	p->argc=0;
	p->argv=(char **)malloc(sizeof(char *));
	p->argv[0]=0;
	for(;;){
		lp=line;
		do{
			if(lp==&line[NLINE] || read(p->fd, lp, 1)!=1){
				PIC_error="Can't read header";
				goto Error;
			}
		}while(*lp++!='\n');
		if(lp==line+1)
			break;
		lp[-1]='\0';
		lp=strchr(line, '=');
		if(lp==0){
			PIC_error="Ill-formed header line";
			goto Error;
		}
		*lp++='\0';
		picputprop(p, line, lp);
	}
	if(strncmp(p->argv[0], "TYPE=", 5)!=0){
		PIC_error="First header line not TYPE=";
		goto Error;
	}
	p->type=p->argv[0]+5;
	for(i=0;PIC_conf[i].type;i++)
		if(strcmp(p->type, PIC_conf[i].type)==0) break;
	if(!PIC_conf[i].type){
		PIC_error="Illegal TYPE";
		goto Error;
	}
	p->rd=PIC_conf[i].rd;
	p->wr=PWR_error;
	p->cl=PIC_conf[i].cl;
	p->line=0;
	p->buf=0;
	p->chan=picgetprop(p, "CHAN");
	nchan=picgetprop(p, "NCHAN");
	if(PIC_conf[i].nchan){
		p->nchan=PIC_conf[i].nchan;
		if(nchan && p->nchan!=atoi(nchan)){
			PIC_error="wrong NCHAN for this TYPE";
			goto Error;
		}
		if(p->chan==0) switch(p->nchan){
		case 1: p->chan="m"; break;
		case 3: p->chan="rgb"; break;
		}
	}
	else if(nchan)
		p->nchan=atoi(nchan);
	else if(p->chan)
		p->nchan=strlen(p->chan);
	else{
		PIC_error="Neither CHAN nor NCHAN in header";
		goto Error;
	}
	if(p->chan==0 && p->nchan<=16)
		p->chan="????????????????"+16-p->nchan;
	window=picgetprop(p, "WINDOW");
	if(window==0){
		PIC_error="No WINDOW in header";
		goto Error;
	}
	if(sscanf(window, "%d%d%d%d", &x0, &y0, &x1, &y1)!=4) goto Error;
	if(x0<x1){
		p->x=x0;
		p->width=x1-x0;
	}
	else{
		p->x=x1;
		p->width=x0-x1;
	}
	if(y0<y1){
		p->y=y0;
		p->height=y1-y0;
	}
	else{
		p->y=y1;
		p->height=y0-y1;
	}
	if(picgetprop(p, "CMAP")){
		p->cmap=malloc(3*256);
		if(p->cmap==0){
			PIC_error="Can't allocate color map";
			goto Error;
		}
		i=0;
		lp=p->cmap;
		while(i!=3*256){
			n=read(p->fd, lp, 3*256-i);
			if(n<=0){
				PIC_error="Can't read color map";
				goto Error;
			}
			i+=n;
			lp+=n;
		}
	}
	else
		p->cmap=0;
	command=picgetprop(p, "COMMAND");
	if(command==0){
		sprintf(line, "magically-create %s", file);
		command=line;
	}
	if(PIC_command==0){
		PIC_command=malloc(strlen(command)+1);
		if(PIC_command)
			strcpy(PIC_command, command);
	}
	else{
		PIC_command=realloc(PIC_command, strlen(PIC_command)+strlen(command)+2);
		if(PIC_command){
			strcat(PIC_command, "\n");
			strcat(PIC_command, command);
		}
	}
	return p;
}
