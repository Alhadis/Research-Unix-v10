
	/*	some functions from the UNIX manual section 2
	  	
	  	INCOMPLETE: contains only declarations of functions that
		(1) is not declared with their data structures
		(2) are identically declared in UNIX5.2 and bsd4.2
	*/
/* extern char* ctermid(char*); */

extern int system(char*);
extern int access(char*, int);
extern int acct(char*);
/* extern alarm(unsigned); */
/* extern char* brk(int); */
extern char* sbrk(int);
extern int chdir(char*);
extern int chmod(char*, int);
extern int chown(char*, int, int);
extern int close(int);
extern int creat(char*, int);
extern int dup(int);
extern int execl(char* ...);
extern int execv(char*, char**);
extern int execle(char* ...);
extern int execve(char*, char**, char**);
extern void exit(int), _exit(int);
extern int fork();
extern int getpid();
extern int kill(int, int);
extern int link(char*, char*);
extern long lseek(int, long, int);
extern int mknod(char*, int, int);
extern int mount(char*, char*, int);
extern int nice(int);
extern int open(char*, int);
extern pause();
extern int pipe(int*);
/* extern void profil(char *, int, int, int); */
extern int setuid(int);
extern int setgid(int);
extern int stime(long*);
extern void sync();
extern int umask(int);
extern int umount(char*);
extern int unlink(char*);
extern int wait(int*);
extern int read(int, char *, int);
extern int write(int, char *, int);
