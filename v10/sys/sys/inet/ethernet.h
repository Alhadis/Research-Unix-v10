#ifndef _ETHERNET_
#define _ETHERNET_

struct etherpup {
     u_char    dhost[6];
     u_char    shost[6];
     u_short   type;
};

#define ETHERPUP_PUPTYPE	0x400
#define ETHERPUP_IPTYPE		0x800
#define ETHERPUP_CHAOSTYPE	0x804
#define ETHERPUP_ARPTYPE	0x806

#endif

#ifndef CHANS_PER_UNIT
#define CHANS_PER_UNIT	8
#endif
