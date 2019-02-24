#include "paint.h"
char brushname[512]="/usr/jerq/lib/paint/brush/dot03";
char brushdir[512]="/usr/jerq/lib/paint/brush";
char picdir[512]="pic";
char texdir[512]="/usr/jerq/lib/paint/tex";
readstate(){
	register FILE *f=fopen(".paintstate", "r");
	register i;
	char line[512];
	if(!f)
		return;
	while(fgets(line, sizeof line, f)){
		if(line[i=strlen(line)-1]=='\n')
			line[i]='\0';
		if(strncmp(line, "brush=", 6)==0)
			strcpy(brushname, line+6);
		else if(strncmp(line, "brushdir=", 9)==0)
			strcpy(brushdir, line+9);
		else if(strncmp(line, "picdir=", 7)==0)
			strcpy(picdir, line+7);
		else if(strncmp(line, "texdir=", 7)==0)
			strcpy(texdir, line+7);
		else if(strncmp(line, "texture=", 8)==0)
			atotex(&tex, line+8);
		else if(strncmp(line, "style=", 6)==0)
			setstyle(atoi(line+6));
		else if(strncmp(line, "operation=", 10)==0)
			setop(atoi(line+10));
		else if(strncmp(line, "textured=", 9)==0)
			settex(atoi(line+9));
		else if(strncmp(line, "linestyle=", 10)==0)
			setlinestyle(atoi(line+10));
	}
	fclose(f);
}
writestate(){
	extern Texture dead;
	Texture *t=cursswitch(&dead);
	register FILE *f=fopen(".paintstate", "w");
	if(!f){
		cursswitch(t);
		return(0);
	}
	fprintf(f, "brush=%s\nbrushdir=%s\npicdir=%s\ntexdir=%s\ntexture=",
		brushname, brushdir, picdir, texdir);
	wrtex(f, &tex);
	fprintf(f, "style=%d\noperation=%d\ntextured=%d\nlinestyle=%d\n",
		curstyle, curop, textured, curlinestyle);
	fclose(f);
	cursswitch(t);
	return(1);
}
char *base(s, c)
char *s;
{
	char *rindex(), *t=rindex(s, c);
	return(t?t+1:s);
}
char *nomung(s)
register char *s;
{
	static char buf[512];
	register char *t=buf;
	while(*s){
		if((*s&0200)==0)
			*t++=*s;
		s++;
	}
	*t='\0';
	return(buf);
}
NMitem Istyle[], Ioperation[];
showstate(){
	char line[100];
	static char prev[100];
	strcpy(line, base(brushname, '/'));
	strcat(line, ", ");
	strcat(line, nomung(Istyle[curstyle].text+1));
	strcat(line, ", ");
	strcat(line, nomung(Ioperation[curop].text+1));
	strcat(line, textured?", texture, ":", no texture, ");
	strcat(line, linestring);
	if(strcmp(prev, line)!=0){
		msg(line);
		strcpy(prev, line);
	}
}
