#ifndef __SYSENT_H
#define __SYSENT_H 1

	/*	some functions from the UNIX manual section 2
	  	
	  	INCOMPLETE: contains only declarations of functions that
		(1) is not declared with their data structures
		(2) are identically declared in UNIX5.2 and bsd4.2
	*/
extern "C" {
	
	/* extern const char* ctermid(const char*); */
	
	extern int system(const char*);
	extern int access(const char*, int);
	extern int acct(const char*);
	extern alarm(unsigned);
	/* extern const char* brk(int); */
	extern const char* sbrk(int);
	extern int chdir(const char*);
	extern int chmod(const char*, int);
	extern int chown(const char*, int, int);
	extern int close(int);
	extern int creat(const char*, int);
	extern int dup(int);
	extern int execl(const char* ...);
	extern int execv(const char*, const char**);
	extern int execle(const char* ...);
	extern int execve(const char*, const char**, const char**);
	extern void exit(int), _exit(int);
	extern int fork();
	extern int getpid();
	extern int ioctl(int, int ...);
	extern int kill(int, int);
	extern int link(const char*, const char*);
	extern long lseek(int, long, int);
	extern int mknod(const char*, int, int);
	extern int mount(const char*, const char*, int);
	extern int nice(int);
	extern int open(const char*, int);
	extern pause();
	extern int pipe(int*);
	/* extern void profil(const char*, int, int, int); */
	extern int setuid(int);
	extern int setgid(int);
	extern int sleep(int);
	extern int stime(long*);
	extern void sync();
	extern int umask(int);
	extern int umount(const char*);
	extern int unlink(const char*);
	extern int wait(int*);
	extern int read(int, const char*, int);
	extern int write(int, const char*, int);
	
}

#endif
