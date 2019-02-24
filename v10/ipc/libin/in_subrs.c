#include <ctype.h>
#include <sys/inet/in.h>
#include <ipc.h>

extern in_addr in_aton();

/* Get the next token in a string, returning a pointer the the byte
 * following the token.
 */
char *
in_getw(buf, w)
char *buf, *w;
{
	*w = 0;
	while(isspace(*buf)) buf++;
	if(*buf == '\0')
		return(0);
	while(!isspace(*buf) && *buf)
		*w++ = *buf++;
	*w = 0;
	return(buf);
}

/* get the network that a host is on */
in_addr
in_netof(x)
in_addr x;
{
	if(IN_CLASSC(x))
		return(x&IN_CLASSC_NET);
	else if(IN_CLASSB(x))
		return(x&IN_CLASSB_NET);
	else
		return(x&IN_CLASSA_NET);
}

/*
 *  get the subnetwork that a host is on.  if the mask is not a superset
 *  the classmask, just use the classmask.
 */
in_addr
in_subnetof(x, mask)
	in_addr x;
	in_addr mask;
{
	in_addr cmask;

	if(IN_CLASSC(x))
		cmask = IN_CLASSC_NET;
	else if(IN_CLASSB(x))
		cmask = IN_CLASSB_NET;
	else
		cmask = IN_CLASSA_NET;
	if((cmask&mask) != cmask)
		return x & cmask;
	else
		return x & mask;
}

/*
 *  get the ip address for `name' on same net as `naddr'
 */
in_addr
in_addronnet(name, naddr, mask)
	char *name;
	in_addr naddr;
	in_addr mask;
{
	in_addr taddr;
	Qset *sp, *tsp;
	Qtuple *tp;

	/*
	 *  get the subnetwork naddr is on
	 */
	naddr = in_subnetof(naddr, mask);

	/*
	 *  if `name' is numeric, don't call the name server
	 */
	if(*name >= '0' && *name <= '9'){
		taddr = in_aton(name);
		if(in_subnetof(taddr, mask)!=naddr)
			taddr = 0;
		return taddr;
	}

	/*
	 *  ask name server for a set of possibilities
	 */
	sp = qset(name, (char *)0);
	if (!sp)
		return 0;

	/*  
	 *  Look for an inet address that matches the network.
	 *  If none found return 0.
	 */
	for(tsp=sp; tsp; tsp=tsp->next) {
		for(tp=tsp->this; tp; tp=tp->next)
			if(tp->type && strcmp(tp->type, "in")==0){
				taddr = in_aton(tp->value);
				if(in_subnetof(taddr, mask)==naddr){
					freeQset(sp);
					return taddr;
				}
			}
	}
	freeQset(sp);
	return 0;
}
