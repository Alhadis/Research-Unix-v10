#define PIOC		('p'<<8)

#define PIOCGETPR	(PIOC|1)	/* read struct proc */
#define PIOCOPENT	(PIOC|2)	/* open text file for reading */
#define PIOCEXCLU	(PIOC|3)	/* mark text for exclusive use */

#define PIOCSTOP	(PIOC|4)	/* send STOP signal and... */
#define PIOCWSTOP	(PIOC|5)	/* wait for process to STOP */
#define PIOCRUN		(PIOC|6)	/* make process runnable */

#define PIOCSMASK	(PIOC|7)	/* set signal trace bit mask */
#define PIOCCSIG	(PIOC|8)	/* clear current signal */
#define PIOCKILL	(PIOC|9)	/* send signal */

#define PIOCSEXEC	(PIOC|10)	/* stop on exec */
#define PIOCREXEC	(PIOC|11)	/* run on exec */

#define PIOCNICE	(PIOC|12)	/* set nice priority */
#define PIOCSSIG	(PIOC|13)	/* set current signal */
