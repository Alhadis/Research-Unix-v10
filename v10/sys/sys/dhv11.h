/*
 * structures for DHV11 serial interface driver
 */

struct	dhv {
	short state;
	short flags;
	struct dhvreg *regs;
	struct queue *rdq;
	char adno;
	char line;
	char ispeed, ospeed;
	short lctl;
	struct block *oblock;
};
