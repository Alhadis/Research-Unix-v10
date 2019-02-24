#define	ETHERALEN	6	/* bytes in an ethernet address */

struct etherpup {
     unsigned char    dhost[ETHERALEN];
     unsigned char    shost[ETHERALEN];
     unsigned short   type;
};

#define ETHERPUP_IPTYPE	      0x800
#define ETHERPUP_CHAOSTYPE    0x804
#define ETHERPUP_ARPTYPE      0x806
