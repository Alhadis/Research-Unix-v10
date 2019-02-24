/*
 * I/O space registers for MicroVAXes
 *
 * a chunk of virtual address space containing
 * - a page of CPU regs
 * - a page of watch chip or NVRAM
 * - maybe another page of cache-related regs
 * - 8192*4 bytes of Q-bus IO map
 * - 8192 bytes of Q-bus IO registers
 *
 * someday, if MicroVAXes get more complicated,
 * this will have to be several files
 */

/*
 * cpu registers: physical 0x20080000
 */
struct cpuregs {
	long junk0;		/* bdr or scr -- we don't use it */
	long mser;		/* II: memory system errors */
	long cear;		/* II: addr of cpu mem err */
	long dear;		/* addr of dma mem err */
	long qbm;		/* III: q-bus map base */
	long junk1[75];
	long memcsr16;		/* III: mem error flags */
	long memcsr17;		/* III: mem error controls */
	long junk2[46];
};

#define	dser	mser		/* III: dma system error */
#define	qbear	cear		/* III: Q-bus err addr */

/*
 * second-level cache register: physical 0x20084000
 * second-level cache image, for flushing: physical 
 * MicroVAX III only
 */

#define	CACHESIZE	(64*1024/4)
struct cacheregs {
	long cacr;		/* second-level cache control */
	long junk[127];
	long cd[CACHESIZE];	/* the cache itself */
};

/*
 * watch chip registers: physical 0x200b8000
 * MicroVAX II only
 * some belong to the clock,
 * some are just handy NVRAM
 * only the low 8 bits of each short is valid
 */
struct watchregs {
	short sec;		/* seconds */
	short junk0;		/* seconds alarm */
	short min;		/* minutes */
	short junk1;		/* minutes alarm */
	short hr;		/* hours */
	short junk2;		/* hours alarm */
	short junk3;		/* day of week */
	short day;		/* day of month */
	short mon;		/* month */
	short yr;		/* year */
	short csra;
	short csrb;
	short csrc;
	short csrd;
	short cpmbx;		/* console mailbox */
	short ram[49];		/* miscellaneous ram */
	long junk[96];
};

/*
 * plain old NVRAM: physical 0x20140400
 * MicroVAX III only
 * really two pages, but we only bother with one
 */

struct nvram {
	short cpmbx;		/* console mailbox */
	short m[255];		/* miscellaneous ram */
};

#define	NQMREG	8192
struct ubadev {			/* `ubadev' sic */
	long mreg[NQMREG];	/* map regs */
	char ioreg[8192];	/* Q-bus io space */
};

struct iomflow {
	struct cpuregs c;
	struct watchregs w;
	struct ubadev u[1];
};

struct iomfair {
	struct cpuregs c;
	struct nvram w;
	struct cacheregs d;
	struct ubadev u[1];
};
