#include "hped.h"


extern char docdir[];

char *hlp[] = {
"Button 1 selects from the menu at left and it is also used",
"to draw or point to objects.",
"Button 3 selects from a list of menus under the cursor and",
"the selected menu is displayed at left. Button 3 is also",
"used for termination of drawing actions.",
"When button 2 is pressed over a menu listing it provides",
"on-line documentation. Otherwise it returns to the basic menu",
" ",
"This table lists the contents of the submenus that 3 selects.",
"basic: type to host, plot host/blit, bring in, zoom, exit",
"ch size: expand, shrink, rotate, flip, elongate", 
"move: del obj, copy obj, move obj, advanced object operations",
"draw: make grid, lines, label, text, circle, box, oval,",
"      make a family",
"reshape: spline, arrow, close, mv point, del pnt, ",
"         dissolve a family, or restore it",
"refine:  edit text, line form (dash, etc)",
"shade: fill polygons with texture, unfill",
"remote: for experienced users only!",
" ",
"A (t) after a menu entry stands for `toggle'",
"+",	/* for portability to Unix V */
NULL};

char * men_name[] = {
	"basic",
	"ch_size",
	"move",
	"draw",
	"reshape",
	"refine",
	"shade",
	"color",
	"remote",
	NULL};

send_help(n,s)
	short n;	/* menu number active on jerq */
	char *s;	/*
			* menu label from jerq, except:
			if shade menu label is "shade", if HELP
			is selected label is "all"
			*/
{
	char **hp;
	char aa[128];
	char mname[128];
	char *ap, *np;
	char found;
	FILE *fh;

	if(!(strncmp(s,"all",3))) {
		strcpy(aa,docdir);	strcat(aa,"news");
		if((fh=fopen(aa,"r"))!=NULL) {
			found = 0;
			while(fgets(aa,128,fh) != 0){
				if(!found++) sendstring("NEWS!");
				if(np = pindex(aa,'\n')) *np = 0;
				sendstring(aa);
			}
			fclose(fh);
			if(found) sendstring("END OF NEWS");
		}

		for(hp = hlp;**hp != '+';hp++) sendstring(*hp);
	}
	else {
		if(0<=n && n<9) strcpy(mname,men_name[n]);
		else strcpy(mname,"?");
		if(!strcmp("GEN",s))
		sprintf(aa,"Summary documentation for menu `%s':",mname);
		else
		sprintf(aa,"Documentation for command `%s' in menu `%s':",
			s,mname);
		sendstring(aa);
		strcpy(aa,docdir);	strcat(aa,mname);
		if((fh=fopen(aa,"r"))==NULL) {
			rrecord("%s not found",aa);
			sendstring("Sorry our on-line documentation is not ready yet");
		}
		else {
			found = 0;
			while(fgets(aa,128,fh) != 0){
				ap = pindex(aa,'	');
				if(ap) {
					*ap = 0;
					if(!strcmp(aa,s)) { /* match*/
						np = pindex(ap+1,'\n');
						if(np) *np = 0;
						sendstring(ap+1);
						found = 1;
					}
					else if(found) break;
				}	
			}
			if(!found) sendstring("No entry in on-line documentation");
			fclose(fh);
		}
	}
	sendstring(" ");
	sendstring("Press a button to resume work.");
	s_short(0);
	fflush(toblit);
}
