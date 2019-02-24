/* Copyright 1989 by AT&T Bell Laboratories */
#include "tags.h"
#include "prof.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#ifdef V9
#include <sys/filio.h>
#include <sys/ttyio.h>
#else
#include <sys/ioctl.h>
#endif

extern int datalist[];

struct {int tag; char s[4];} div_s = {mak_desc(3,tag_string), "Div\0"};

int div_e0[]={mak_desc(2,tag_record),
	     1,
	     (int) (div_e0+4),
	     mak_desc(1,tag_array),
	     (int) div_s.s};

struct {int tag; char s[8];} overflow_s = {mak_desc(8,tag_string), "Overflow"};

int overflow_e0[]={mak_desc(2,tag_record),
	     1,
	     (int) (overflow_e0+4),
	     mak_desc(1,tag_array),
	     (int) overflow_s.s};

struct {int tag; char s[12];} interrupt_s = 
	{mak_desc(9,tag_string), "Interrupt\0\0\0"};

int interrupt_e0[]={mak_desc(2,tag_record),
	     1,
	     (int) (interrupt_e0+4),
	     mak_desc(1,tag_array),
	     (int) interrupt_s.s};

struct {int tag; char s[12];} systemcall_s = 
	{mak_desc(10,tag_string), "SystemCall\0\0"};

int systemcall_e0[]={mak_desc(1,tag_array),
	            (int) systemcall_s.s};

struct {int tag; char s[4];} real_s = 
	{mak_desc(4,tag_string), "Real"};

struct {int tag; char s[12];} unboundTable_s = 
	{mak_desc(12,tag_string), "UnBoundTable"};
int unboundTable_e0[]={mak_desc(1,tag_array),
	            (int) unboundTable_s.s};

int real_e0[]={mak_desc(1,tag_array),
	            (int) real_s.s};

int array0_v[]={mak_desc(0,tag_array)};

int bytearray0_v[]={mak_desc(0,tag_bytearray)};

extern int collected0[];
extern int collectedfrom0[];
extern int current0[];
extern int gcmessages0[];
extern int majorcollections0[];
extern int minorcollections0[];
extern int pstruct0[];
extern int ratio0[];
extern int softmax0[];

struct {int tag, calls, interrupts; char c[4];} gcprof =
    {mak_desc(12,tag_bytearray), 1, 1, "(gc)"};

#ifndef NS32
int fion(fd) int fd;
{static struct stat buf;
 int count[2]; int pos; int r;
 fd >>= 1;
 r=ioctl(fd,FIONREAD,count);
 if (r>=0) return (count[0]<<1)+1;
 if (fstat(fd,&buf)<0) return -1;
 pos = lseek(fd,0,1);
 if (pos<0) pos=0;
 return ((buf.st_size - pos)<<1) + 1;
}
#else
int fion(fd) int fd;
{static struct stat buf;
 int count[2]; int pos; int r;
 fd >>= 1;
 r=fstat(fd,&buf);
 if (r>=0) {
   pos = lseek(fd,0,1);
   if (pos>=0) {
	r = (buf.st_size - pos);
	if (r >= 0) return ((r<<1) + 1);
	}
   }
 r=ioctl(fd,FIONREAD,count);
 if (r>=0) return (count[0]<<1)+1;
 return(-1);
}
#endif NS32

int profvec(arg) struct {int index, newval;} *arg;
{extern int bottom[];
 int k = bottom[arg->index>>1];
 bottom[arg->index>>1] = arg->newval;
 return k;
}

int syst(s) char *s;
{
 return (system(s)<<1)+1;
}

char **environ, **global_argv;
char *argv(arg) int arg;
{char *s=global_argv[arg>>1];
 return s?s:(char*)1;
}
char *envi(arg) int arg;
{char *s = environ[arg>>1];
 return s?s:(char*)1;
}

int time(arg) struct {int *g_usec, *g_sec, *t_usec, *t_sec;} *arg;
{extern g_usec, g_sec, t_usec, t_sec;
  timer();
  *(arg->g_usec) = g_usec;
  *(arg->g_sec) = g_sec;
  *(arg->t_usec) = t_usec;
  *(arg->t_sec) = t_sec;
  return 1;
}

int blastfil;
blast_write(start,end,ptr) int start,end, ptr;
{int i = ptr-start;
 bulletproofWrite(blastfil,&start,4);
 bulletproofWrite(blastfil,&end,4);
 bulletproofWrite(blastfil,&i,4);
 bulletproofWrite(blastfil,start,end-start);
}


extern int cause;
int blas(arg) struct{int filid; int obj;} *arg;
{
 cause = CAUSE_BLAST;
 blastfil = arg->filid >> 1;
 return arg->obj;
}

int salb(arg) struct{int start, end, offset; int words[1];} *arg;
{
 ((int*)arg)[-1] = mak_desc(4,tag_string);
 relocate(arg->start,arg->end,arg->words);
 return (int)(arg->words)+arg->offset;
}

extern int exportFilid;
int expo(filid) int filid;
{exportFilid=filid;
 cause=CAUSE_EXPORT;
 return 1;
}


int exec(arg) struct {char *command; int *fdin, *fdout;} *arg;
{int p1[2], p2[2];
  if (pipe(p1)<0) return -1;
  if (pipe(p2)<0) return -1;
#ifdef V9
  if (fork())
#else
  if (vfork())
#endif
   {close(p1[0]); close(p2[1]);
    *(arg->fdin)= p2[0]*2+1; *(arg->fdout)=p1[1]*2+1;
   }
  else
   {close(p1[1]); close(p2[0]);
    dup2(p1[0],0); dup2(p2[1],1);
    execl("/bin/sh","/bin/sh","-c",arg->command,0);
    _exit(1);
   }
  return 1;
}

int stor(arg) int arg;
{
 cause = CAUSE_STOR;
 return arg;
}

int mtim(fd)
int fd;				/* return (ML) ~1 on error. */
{
   struct stat buf;
   if (fstat(fd>>1, &buf))	/* Non-zero: failure. */
      return(ML_INT(-1));
   else if (buf.st_mtime & 0x80000000)		/* Overflow in ML int? */
      return(ML_INT(-1));
   else
      return(ML_INT(buf.st_mtime));
}

extern int errno;

int errn()
{
    return errno+errno+1;
}

int isat(fd) int fd;
{
    return isatty(fd>>1)*2+1;
}



/* name must be 4 characters exactly */
#define function(ff,nn,i) \
     {mak_desc(3,tag_record),  \
     (int)ff, \
     (int) externlist0[i].str , \
     (int) &(externlist0[(i)+1].func), \
     mak_desc(4,tag_string), \
     nn},
     
struct {int tag,func,name,next,stag; char str[4];} externlist0[]=
{function(fion,"fion",0)
 function(exec,"exec",1)
 function(profvec,"prof",2)
 function(syst,"syst",3)
 function(time,"time",4)
 function(argv,"argv",5)
 function(envi,"envi",6)
 function(blas,"blas",7)
 function(salb,"salb",8)
 function(expo,"expo",9)
 function(stor,"stor",10)
 function(mtim,"mtim",11)
 function(errn,"errn",12)
 function(isat,"isat",13)
 function(0,"xxxx",14)
};
 
extern int runvec[];
extern int errstrings[];

int *cstruct[]={ 
   (int*)mak_desc(20,tag_record),
   runvec,
   div_e0+1,
   interrupt_e0+1,
   overflow_e0+1,   		
   real_e0+1,
   systemcall_e0+1,
   unboundTable_e0+1,
   array0_v+1,
   bytearray0_v+1,
   collected0+1,
   collectedfrom0+1,
   current0+1,
   datalist,
   errstrings+1,
   &(externlist0[0].func),
   gcmessages0+1,
   &gcprof.calls,
   majorcollections0+1,
   minorcollections0+1,
#ifdef V9
   (int*)7,
#else
#ifdef VAX
   (int*)3,
#else
   (int*)5,
#endif
#endif
   pstruct0+1,
   ratio0+1,
   softmax0+1
};


struct {int tag; char s[8];} ovf_s = {mak_desc(8,tag_string), "overflow"};
int float_ovf [] = {mak_desc(2,tag_record), (int)ovf_s.s, (int)(real_e0+1)};

struct {int tag; char s[12];} und_s = {mak_desc(9,tag_string), "underflow"};
int float_und [] = {mak_desc(2,tag_record), (int)und_s.s, (int)(real_e0+1)};

struct {int tag; char s[16];} fdiv_s = {mak_desc(14,tag_string), 
		"divide by zero"};
int float_div [] = {mak_desc(2,tag_record), (int)fdiv_s.s, (int)(real_e0+1)};

struct {int tag; char s[28];} strange_s = {mak_desc(28,tag_string), 
		"strange floating point error"};
int float_str [] = {mak_desc(2,tag_record), (int)strange_s.s, (int)(real_e0+1)};

struct {int tag; char s[20];} emt_s = {mak_desc(19,tag_string), 
		"68881 not installed"};
int float_emt [] = {mak_desc(2,tag_record), (int)emt_s.s, (int)(real_e0+1)};

int exnCode(signal,code) int signal,code;
{
 if(signal==SIGFPE)
 switch(code)
  {
#ifdef FPE_TRAPV_TRAP
   case FPE_TRAPV_TRAP:
#endif
#ifdef FPE_FLTOPERR_TRAP
   case FPE_FLTOPERR_TRAP:
#endif
#ifdef FPE_OPERAND_TRAP
   case FPE_OPERAND_TRAP:
#endif
#ifdef FPE_INTOVF_TRAP
   case FPE_INTOVF_TRAP:
#endif
#ifdef FPE_INTOVF_FAULT
   case FPE_INTOVF_FAULT:
#endif
#ifdef K_INTOVF
   case K_INTOVF:
#endif
            return (int)(overflow_e0+1);
#ifdef FPE_INTDIV_TRAP
   case FPE_INTDIV_TRAP:
#endif
#ifdef FPE_INTDIV_FAULT
   case FPE_INTDIV_FAULT:
#endif
#ifdef K_INTDIV
   case K_INTDIV:
#endif
#ifdef FPE_ZERODIV_TRAP
   case FPE_ZERODIV_TRAP:
#endif
	    return (int)(div_e0+1);
#ifdef FPE_FLTOVF_TRAP
   case FPE_FLTOVF_TRAP:
#endif
#ifdef FPE_FLTOVF_FAULT
   case FPE_FLTOVF_FAULT:
#endif
#ifdef K_FLTOVF
   case  K_FLTOVF:
#endif
#ifdef FPE_OVERFLOW_TRAP
   case FPE_OVERFLOW_TRAP:
#endif
	    return (int)(float_ovf+1);
#ifdef FPE_FLTDIV_TRAP
   case FPE_FLTDIV_TRAP:
#endif
#ifdef FPE_FLTDIV_FAULT
   case FPE_FLTDIV_FAULT:
#endif
#ifdef K_FLTDIV
   case K_FLTDIV:
#endif
#ifdef FPE_DIVZERO_TRAP
   case FPE_DIVZERO_TRAP:
#endif
	    return (int)(float_div+1);
#ifdef FPE_FLTUND_TRAP
   case FPE_FLTUND_TRAP:
#endif
#ifdef FPE_FLTUND_FAULT
   case FPE_FLTUND_FAULT:
#endif
#ifdef FPE_UNDERFLOW_TRAP
   case FPE_UNDERFLOW_TRAP:
#endif
#ifdef K_FLTUND
   case K_FLTUND:
#endif
	    return (int)(float_und+1);
   default:
	    return (int)(float_str+1);
  }
 else if (signal==SIGEMT)
	    return (int)(float_emt+1);
 else if (signal==SIGINT)
	    return (int)(interrupt_e0+1);
}
