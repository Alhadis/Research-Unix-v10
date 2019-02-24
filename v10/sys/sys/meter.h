/*
 * Instrumentation
 */
#define	CPUSTATES	5

#define	CP_USER		0
#define	CP_NICE		1
#define	CP_SYS		2
#define	CP_IDLE		3
#define	CP_QUEUE	4

#ifdef KERNEL
long	cp_time[CPUSTATES];
#endif
