/* a pipe between parent and child */
typedef struct {
	FILE *fp;	/* parent process end */
	int fd;		/* child process end */
} stream;

/* a child process */
typedef struct process{
	stream *std[3];	/* standard fd's */
	int pid;	/* process identifier */
	int status;	/* exit status */
} process;

/* start/stop for a process */
extern process *proc_start();
extern int proc_wait();
extern int proc_free();
extern void stream_free();
extern stream *instream();
extern stream *outstream();
