#include <dmd.h>
#include <5620.h>
#include <font.h>
#include <string.h>
#include "menu.h"


#define MAXLIST 64

Font *monfont;
NMenu *b2;

NMenu *
newmenu()
{
	NMenu *m;

	m = (NMenu *)alloc(sizeof(NMenu));
	m->generator = 0;
	m->prevhit = -1;
	m->prevtop = -1;
	m->item = (NMitem *)alloc((MAXLIST+1)*sizeof(NMitem));
	m->item->text = (char *)0;
	return(m);
}

void addentry(m, l)
NMenu *m;
char *l;
{
	NMitem *ip=m->item;
	int n, found=0;
	char ch, *nextname, *fullname;

	for (; (ch=*l) && (ch==' ' || ch=='\t'); l++) {}
	if (!*l)
		return;
	if ((nextname=strchr(l, '\t')) != (char *)0)
		*nextname++ = '\0';
	for (n=MAXLIST; n>0 && ip->text && !(found=!strcmp(ip->text, l)); ip++, n--) {}

	if (!found && n<=0)		/* no room for a new entry */
		return;
	if (!found) {
		if((fullname=strchr(l, '|')) != (char *)0)
			*fullname++ = '\0';
		else
			fullname = l;
		ip->text = strcpy(alloc(strlen(l)+1), l);
		ip->help = strcpy(alloc(strlen(fullname)+1), fullname);
		ip->next = (NMenu *)0;
		ip->dfn  = ip->bfn = ip->hfn = 0;
		ip->data = 0;
		(ip+1)->text = (char *)0;
	}
	if(!nextname)
		return;
	if(!ip->next) 
		ip->next = newmenu();
	addentry(ip->next, nextname);
}

main()
{
	NMitem *i;
	monfont = &defont;

	b2 = newmenu();

	addentry(b2, "ca/xunet/fs");
	addentry(b2, "astro	alice|dk!alice");
	addentry(b2, "astro	ara|dk!ara");
	addentry(b2, "astro	arend|dk!arend");
	addentry(b2, "astro	dk!arpa");
	addentry(b2, "astro	dk!bowell");
	addentry(b2, "astro	dk!coma");
	addentry(b2, "astro	dk!crab");
	addentry(b2, "astro	dk!dutoit");
	addentry(b2, "astro	dk!encke");
	addentry(b2, "astro	dk!giacobini");
	addentry(b2, "astro	dk!kwee");
	addentry(b2, "astro	dk!pipe");
	addentry(b2, "astro	dk!r70");
	addentry(b2, "astro	dk!research");
	addentry(b2, "astro	dk!seki");
	addentry(b2, "astro	dk!spit");
	addentry(b2, "astro	dk!swift");
	addentry(b2, "astro	tcp!tuttle");
	addentry(b2, "astro	dk!west");
	addentry(b2, "astro	dk!westphal");
	addentry(b2, "astro	dk!wild");
	addentry(b2, "nj/garage/europa");
	addentry(b2, "mercury	dk!balin");
	addentry(b2, "mercury	dk!bashful");
	addentry(b2, "mercury	dk!bifur");
	addentry(b2, "mercury	dk!bofer");
	addentry(b2, "mercury	dk!capek");
	addentry(b2, "mercury	dk!doc");
	addentry(b2, "mercury	dk!dopey");
	addentry(b2, "mercury	dk!dori");
	addentry(b2, "mercury	dk!gimli");
	addentry(b2, "mercury	dk!gloin");
	addentry(b2, "mercury	dk!grumpy");
	addentry(b2, "mercury	dk!happy");
	addentry(b2, "mercury	dk!hunny");
	addentry(b2, "mercury	dk!icarus");
	addentry(b2, "mercury	dk!iota");
	addentry(b2, "mercury	dk!jones");
	addentry(b2, "mercury	dk!kanga");
	addentry(b2, "mercury	dk!sapir");
	addentry(b2, "mercury	dk!siriusb");
	addentry(b2, "mercury	dk!sleepy");
	addentry(b2, "mercury	dk!sola");
	addentry(b2, "mercury	dk!sweet");
	addentry(b2, "mercury	dk!tattoo");
	addentry(b2, "mercury	dk!thorin");
	addentry(b2, "phone	dk!atomic");
	addentry(b2, "phone	dk!fry");
	addentry(b2, "phone	dk!gauss");
	addentry(b2, "phone	dk!shannon");
	addentry(b2, "phone	dk!sid");
	addentry(b2, "nj/vlsi/araki");
	addentry(b2, "pa/ram/most");

	request(MOUSE);
	while(wait(MOUSE)) {
		if (button2()) {
			i = mhit(b2, 2, 0);
			if(i)
				lprintf("%s\n", i->help);
			else
				lprintf("nope..\n");
		}
	}
}
