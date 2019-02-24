/* Copyright 1989 by AT&T Bell Laboratories */
#include <signal.h>
#include <sys/types.h>
#ifdef BSD
#include <sys/time.h>
#include <sys/resource.h>
#endif

#ifdef MIPS
#include <sys/syscall.h>
#include <mips/cachectl.h>
#include <errno.h>
#endif

#include "tags.h"
#include "descriptor.h"

#define refcell(z) int z[2] = {mak_desc(1,tag_array), ML_INT(0)};

#ifdef NeXT

#include <c.h>			/* TRUE and FALSE */
#include <sys/kern_return.h>	/* KERN_whatever */
#include <mach.h>		/* type definitions */

#endif NeXT

refcell(collected0)
refcell(collectedfrom0)
refcell(current0)
refcell(gcmessages0)
refcell(majorcollections0)
refcell(minorcollections0)
refcell(pstruct0)
refcell(ratio0)
refcell(softmax0)

#define collected (collected0[1])
#define collectedfrom (collectedfrom0[1])
#define current (current0[1])
#define gcmessages (gcmessages0[1])
#define majorcollections (majorcollections0[1])
#define minorcollections (minorcollections0[1])
#define pstruct (pstruct0[1])
#define ratio (ratio0[1])
#define softmax (softmax0[1])

int arenabase;               /* bottom of the heap */
int arenasize = 0;           /* heap starts empty */
int new_size = 1024 * 1024;  /* default heap size of 1 megabyte */
int arstart;                 /* beginning of main arena */
int arend;                   /* end of main arena, and the heap */
int old_high;                /* marks end of persistent heap */
int new_high;
int new_new_high;
int lastbreak;

int cause, fault_code;
int bottom;                 /* saved stack-pointer */
int fpsave;		    /* saved frame pointer */
int saved_pc_relative;
int saved_pc, saved_dataptr, saved_exnptr, saved_storeptr=1, saved_limit;
#ifdef SPARC
extern int saved_ptrs[], saved_nonptrs[]; /* allocated in SPARC.prim.s */
#else
int saved_ptrs[32], saved_nonptrs[32];
#endif

int store_preserve=1, preserving=0;

extern int handle_c[], return_c[];

apply(func,arg) int func, arg;
{int i;
    saved_exnptr = (int) handle_c;
    saved_ptrs[0] = arg;
    saved_ptrs[1] = (int) return_c;
    saved_ptrs[2] = func;
    saved_pc = *(int*)func;
    for(i=3;i<32;i++) saved_ptrs[i]=0;
    runML();
    return saved_ptrs[0];
}

int exportFilid;

runML()
{int i;
 for(;;)
      {cause=0;
#ifdef MIPS
       cacheflush((char *) arenabase, arenasize, ICACHE);
#endif
       restoreregs();
       switch(cause)
	{case CAUSE_EXN: return uncaught(saved_ptrs[0]);
	 case CAUSE_GC:  callgc0(); break;
	 case CAUSE_RET: return;
	 case CAUSE_EXPORT: saved_pc=0;
			    for(i=2;i<32;i++) saved_ptrs[i]=0;
			    callgc0(); callgc0();
			    saved_pc = *(int*)saved_ptrs[1];
			    export(exportFilid);
			    saved_ptrs[0]=1;
			    break;
	 case CAUSE_BLAST: saved_pc=0;
	                   callgc0();
	                   saved_pc = *(int*)saved_ptrs[1];
	                   break;
	 case CAUSE_STOR:  saved_pc=0;
	                   callgc0();
			   preserving = (saved_ptrs[0]!=1);
			   saved_ptrs[0]= uniq(store_preserve);
			   store_preserve=1;
	                   saved_pc = *(int*)saved_ptrs[1];
	                   break;
	 case CAUSE_FAULT:   
			   for(i=0;i<32;i++) saved_ptrs[i]=0;
			   saved_ptrs[0]=fault_code;
			   saved_ptrs[1]=saved_exnptr;
	                   saved_pc = *(int*)saved_ptrs[1];
			   break;
	}
      }
}


#define pagesize 8192

#ifdef NeXT

/**
 ** This implements sbrk/brk using vm_allocate/vm_deallocate.
 ** Arguments are assumed to be page multiples, and the argument
 ** to brk is assumed to be just after the desired breakpoint.
 **
 ** No relationship between the mapped region and the rest of the
 ** process image is guaranteed, but it is expected that the region
 ** will follow the end of data/bss.
 **
 ** Works with NeXT Mach (software release 0.9).  5/15/89
 **
 ** James William O'Toole Jr.
 **
 **/

extern vm_task_t task_self_;

int mach_sbrk_needsinit = TRUE;
int mach_maplimit = 0;
int mach_brkpt = 0;
int mach_quant = 0x800000;
int big_heap = 0x2000000;

static int sbrk(incr)
int incr;
{
        if (incr) die("sbrk called with nonzero value");
	if (mach_sbrk_needsinit != FALSE) {
		if (vm_allocate(task_self_, &mach_brkpt, big_heap,TRUE)
		    != KERN_SUCCESS)
			die("vm_allocate failed");
		mach_maplimit = mach_brkpt + big_heap;
		mach_sbrk_needsinit = FALSE;
		}
	return(mach_brkpt);
}

static int brk(pos)
int pos;
{
 if (pos>mach_maplimit) return KERN_FAILURE;
 return KERN_SUCCESS;
}

#endif NeXT

init_gc()
{
 arenabase=sbrk(0);
 lastbreak=arenabase;
 increase_heapsize();
 old_high = arenabase;
 arstart = arenabase+arenasize/2;
 saved_dataptr = (int)arstart;
 saved_limit = arenabase+arenasize-4096;
}

#ifdef ADVICE
struct timeval zero, ogtime, ostime, otime;
int getting_advice;
int old_size;
int old_minorcount;
#endif

restarter()
{extern int edata;
 resettimers();
 lastbreak = (int)&edata;
 getmore_restart();
#ifdef ADVICE
 ostime=zero; otime=zero; ogtime=zero;
 getting_advice=1;
 initadvice();
#endif
 saved_dataptr = (int)arstart;
 saved_limit = lastbreak-4096;
 setupsignals();
 saved_ptrs[0]=3;
 runML();
#ifdef ADVICE
 call_endadvice();
#endif
 _exit(0);
}

callgc0()
{int i; int *roots[40]; int **rootsptr = roots;
#ifdef	V9
 int (*inthandle)();
 inthandle = signal(SIGINT,SIG_IGN);
#endif
 suspendtimers();
 *rootsptr++ = &pstruct;
 *rootsptr++ = &saved_exnptr;
 for(i=0;i<32;i++) *rootsptr++ = saved_ptrs+i;
  {extern int gcprof[];
   int *currentsave = (int *) current;
   current = (int) (gcprof+1);
   *rootsptr++ = (int *) &currentsave;
   *rootsptr++ = (int *) &store_preserve;
   *rootsptr++ = (int *) &saved_pc;
   *rootsptr=0;
   i=0;
   callgc(roots,&saved_dataptr,saved_storeptr);
   saved_storeptr=1;
   current = (int) currentsave;
  }
 restarttimers();
#ifdef V9
 signal(SIGINT,inthandle);
#endif
}

int getmore_die(){die("bug: insufficient to_space\n");}

int amount_desired;

int decrease_heapsize()
{int p = arenabase+new_size;
 p = (p + pagesize-1 ) & ~(pagesize-1);
 if (p<lastbreak)
    {brk(p);
     arenasize = p-arenabase;
     if (gcmessages >= ML_INT(2))
        chatting("\n[Decreasing heap to %dk]\n",arenasize/1024);
     lastbreak=p;}
 return lastbreak;}

int increase_heapsize() /* new_size > arenasize */
{int p = arenabase+new_size;
 RESTART:
 p = (p + pagesize-1 ) & ~(pagesize-1);
 if (p==lastbreak)
    {if (gcmessages >= ML_INT(2)) chatting("\nWarning: can't increase heap\n");
     return p;}
 else if (brk(p))
         {if (gcmessages >= ML_INT(3))
	     chatting("\nWarning: must reduce heap request\n");
          p=(lastbreak+(p-pagesize))/2; goto RESTART;}
      else {lastbreak=p;
            arenasize = p-arenabase;
	    if (gcmessages >= ML_INT(2))
               chatting("\n[Increasing heap to %dk]\n",arenasize/1024);
            return p;}
 }

int compute_new_size(live_size) 
   int live_size;
{int new_size,  gamma = (ratio<7?7:ratio)>>1,  max = softmax>>1;
  if (2000000000 / gamma < live_size)
          new_size = 2000000000;
          else new_size=live_size*gamma;
 if (max < new_size)
    {int new = 3*live_size;
     new_size = (new>max?new:max);}
 return new_size;
}

int getmore_must()
{int oldsize=arenasize;
 int live_size = amount_desired+arenasize+arenabase-old_high;
 new_size=compute_new_size(live_size);
 {int r=increase_heapsize();
#ifdef ADVICE
  while (oldsize==arenasize) 
    {chatting("\nCan't get more memory; waiting\n");
     sleep(10);
     chatting("Trying again\n");
     r=increase_heapsize();
    }
#else
  if (oldsize==arenasize) die("\nRan out of memory");
#endif
  return r;
 }
}

getmore_restart()
{int live_size = old_high - arenabase;
 int a = 0;
 int x = gcmessages;
 gcmessages=ML_INT(0);
 new_size=compute_new_size(live_size);
 do {increase_heapsize();
     if (arenasize==a) die("Can't get enough memory to start ML\n");
     a=arenasize;
   } while (arenasize < 3*live_size);
 gcmessages=x;
}

callgc(misc_roots,  /* vector of ptrs to extra root words */
       arptr,	    /* place to put new freespace pointer */
       store_list   /* list of refs stored into */
      )
    int ***misc_roots; int *arptr; int **store_list;
{int amount_desired;
 arend = arenabase+arenasize;
 if (cause==CAUSE_EXPORT || cause==CAUSE_STOR) amount_desired = 0;
           else amount_desired = 4+arend-(*arptr);
 if (arstart== *arptr) new_high = old_high; /* no minor needed */
 else 
 {if (gcmessages >= ML_INT(3)) chatting("\n[Minor collection...");
  starttime();
  gc(arstart,arend,
    old_high,arstart,
    old_high,
    &new_high,
    misc_roots,store_list,
    getmore_die, 0);
    {int a = new_high-old_high, b =(*arptr)-arstart, msec=endtime();
     if (gcmessages >= ML_INT(3))
	chatting(" %d%% used (%d/%d), %d msec]\n", (100*a)/b, a, b, msec);
     collected += 2*((a+512)/1024); /* round to nearest k */
     collectedfrom += 2*((b+512)/1024);
     minorcollections+=2;
    }
#ifdef GCDEBUG
 checkup(arstart,new_high);
 clear(new_high,arenabase+arenasize);
#endif
   }
{int need_major = 0; int was_preserving; int gamma = (ratio<7?7:ratio)>>1;
  if (cause==CAUSE_EXPORT || cause==CAUSE_BLAST) need_major = 1;
 else {int cut = arenasize-arenasize/gamma;
       int max = softmax>>1;
       int halfmax = max/2;
       int halfsize = arenasize/2;
       cut = (cut<halfmax?cut:halfmax);
       cut = (cut>halfsize?cut:halfsize);
       if (new_high+amount_desired > arenabase+cut) need_major = 1;
       else {int live_size = amount_desired+new_high-old_high;
#ifdef ADVICE
          if ((arenabase+arenasize-new_high)/2 <= amount_desired*3+100) need_major=1;
          if (minorcollections > old_minorcount+200) need_major=1;
#else
             new_size=compute_new_size(live_size);
	     if (new_size > arenasize
		         && (increase_heapsize()-new_high)/2 <= amount_desired)
		 need_major = 1;
#endif ADVICE
	   }}
 if (cause==CAUSE_BLAST) old_high=new_high;
 if (need_major)
    {if (gcmessages >= ML_INT(1)) chatting("\n[Major collection...");
     starttime();
     was_preserving=preserving; preserving=0;
     if (gc(arenabase,old_high,
	old_high,arenabase+arenasize,
	new_high,
	&new_new_high,
        misc_roots,1,
        getmore_must, cause==CAUSE_BLAST? &saved_ptrs[0]: 0))
       {moveback(old_high,new_new_high,
	      arenabase,
	      misc_roots);
        {int a = new_new_high-new_high, 
	      b = new_high-arenabase, msec=endtime();
         if (gcmessages >= ML_INT(1))
        	chatting(" %d%% used (%d/%d), %d msec]\n",(100*a)/b, a, b, msec);
         collected += 2*((a+512)/1024);
         collectedfrom += 2*((b+512)/1024);
         majorcollections+=2;}
	{int live_size = amount_desired+new_new_high-old_high;
         old_high=arenabase+new_new_high-old_high;
#ifdef ADVICE
	 new_size = ask_new_size(live_size);
#else
	 new_size = compute_new_size(live_size);
#endif
	 if (new_size > arenasize)
	    {int end = increase_heapsize();
	     if ((end-old_high)/2 <= amount_desired)
	        die("\nRan out of memory\n");}
#ifdef ADVICE
         else if (new_size < arenasize) decrease_heapsize();
	 old_size = arenasize;
	 old_minorcount = minorcollections;
#else
         else if (new_size < (arenasize/4)*3) decrease_heapsize();
#endif
       }}
       else {endtime(); if (gcmessages >= ML_INT(1)) chatting("abandoned]\n");}
    preserving=was_preserving;
   }
 else old_high=new_high;
 }
 arend = arenabase+arenasize;
 arstart = (((arend+old_high)/2)+3)&(~3);
 (*arptr) = arstart;
 saved_limit = arend-4096;
#ifdef V9
 { extern int ghandle();
   signal(SIGFPE, ghandle);
 }
#endif	/* V9 */
}

#ifdef GCDEBUG
clear(low,high) int *low, *high;
{int *i;
 chatting("clearing new space...  ");
 for(i=low; i<high; i++) *i=0;
 chatting("done\n");
}

int *descriptor;
checkup(low,high)
int *low,*high;
{int *i,*j;
 chatting("checking to_space...  ");
 i = low;
 while (i < high) {
   descriptor = i;
   switch(get_tag(i)) {
        case tag_backptr:
		chatting("Uncool backpointer at %x in to_space\n",i);
		exit(0);
		break;
	case tag_embedded:
		chatting("Uncool embedded tag at %x in to_space\n",i);
		exit(0);
		break;
	case tag_string:
	case tag_bytearray:
		i += (get_len(i)+7)>>2;
		break;
	case tag_record:
	case tag_array:
		j = i + get_len(i) + 1;
		while(++i < j) {
		 if (*i & 1) continue;
		 else if ((int*)*i > high) {
			 chatting("Uncool pointer %x at %x\n", *i,i);
			 chatting("Descriptor is at %x\n",descriptor);
		      }
		 else if ((int*)*i < low) {
			 chatting("Uncool pointer %#x at %#x\n", *i,i);
			 chatting("Descriptor is at %#x\n",descriptor);
		      }
		}
		break;
	case tag_forwarded:
		chatting("Uncool forwarding tag at %x in to_space\n",i);
		exit(0);
		break;
	default: /* function pointers */
		chatting("Unexpected even tag %d at %x in to_space\n",
			 get_tag(i),i);
		exit(0);
		break;
	      }
 }
 chatting("done\n");
}
#endif	/* GCDEBUG */

int g_sec,g_usec,t_sec,t_usec;
#ifdef BSD
struct rusage r1, r2;

starttime()
{getrusage(0,&r1);}

endtime()
{
 int sec,usec;
 getrusage(0,&r2);
 sec = r2.ru_utime.tv_sec-r1.ru_utime.tv_sec;
 usec = r2.ru_utime.tv_usec-r1.ru_utime.tv_usec;
 if (usec < 0) {sec--; usec += 1000000;}
 return (usec/1000 + sec*1000);
}

struct rusage garbagetime,timestamp,s1,s2;

suspendtimers()
{
 getrusage(0,&s1);
}

restarttimers()
{
 int sec,usec;
 getrusage(0,&s2);
 sec = s2.ru_utime.tv_sec-s1.ru_utime.tv_sec+garbagetime.ru_utime.tv_sec;
 usec = s2.ru_utime.tv_usec-s1.ru_utime.tv_usec+garbagetime.ru_utime.tv_usec;
 if (usec < 0) {sec--; usec += 1000000;}
 else if (usec > 1000000) {sec++; usec -= 1000000;}
 garbagetime.ru_utime.tv_sec = sec;
 garbagetime.ru_utime.tv_usec = usec;
}

resettimers()
{
 garbagetime.ru_utime.tv_sec = 0;
 garbagetime.ru_utime.tv_usec = 0;
 s2.ru_utime.tv_sec = s1.ru_utime.tv_sec = 0;
 s2.ru_utime.tv_usec = s1.ru_utime.tv_usec = 0;
 r2.ru_utime.tv_sec = r1.ru_utime.tv_sec = 0;
 r2.ru_utime.tv_usec = r1.ru_utime.tv_usec = 0;
 collected = collectedfrom = minorcollections = majorcollections = ML_INT(0);
}

timer()
{
 getrusage(0,&timestamp);
 g_usec = garbagetime.ru_utime.tv_usec * 2 + 1;
 g_sec = garbagetime.ru_utime.tv_sec * 2 + 1;
 t_usec = timestamp.ru_utime.tv_usec * 2 + 1;
 t_sec = timestamp.ru_utime.tv_sec * 2 + 1;
}

#ifdef ADVICE
struct timeval subt(a,b) struct timeval a,b;
{struct timeval x;
 x.tv_sec = a.tv_sec - b.tv_sec;
 x.tv_usec = a.tv_usec - b.tv_usec;
 if (x.tv_usec<0) {x.tv_sec--; x.tv_usec+=1000000;}
 else if (x.tv_usec > 1000000) {x.tv_sec--; x.tv_usec+=1000000;}
 return x;
}

struct timezone zone;

int milli(t) struct timeval t;
{
    return t.tv_sec*1000 + t.tv_usec/1000;
}

int ask_new_size(live_size) int live_size;
{int answer;
 int s0 = arenabase+2*live_size;
 int sf = /* (old_size?old_size:arenasize) */ arenasize - 2*live_size;
 int delta; struct rusage adv; struct timeval newgtime, newttime, systime;
 getrusage(0,&adv);
 newgtime = subt(subt(adv.ru_utime,s1.ru_utime),
	         subt(ogtime,garbagetime.ru_utime));
 newttime = subt(subt(adv.ru_utime,otime),newgtime);
 systime = subt(adv.ru_stime,ostime);
 ostime=adv.ru_stime;
 otime=adv.ru_utime;
 ogtime = subt(garbagetime.ru_utime,subt(s1.ru_utime,adv.ru_utime));
 if ((!getting_advice) || 
     memadvice(s0,sf,milli(newttime),milli(newgtime),0,
	       live_size,milli(systime),&answer))
    return compute_new_size(live_size);
 else if (answer < live_size/2) return 5*live_size/2;
     else return answer+2*live_size;
}

call_endadvice()
{
 int delta; struct rusage adv; struct timeval newgtime, newttime, systime;
 getrusage(0,&adv);
 newgtime = subt(garbagetime.ru_utime,ogtime);
 newttime = subt(subt(adv.ru_utime,otime),newgtime);
 systime = subt(adv.ru_stime,ostime);
 if (getting_advice)
    endadvice(milli(newttime),milli(newgtime),milli(systime));
}



#endif
#endif	/* BSD */

#ifdef V9
#include <sys/times.h>
struct tms t1,t2;

starttime()
{times(&t1);}

endtime()
{
 int hzsec;
 times(&t2);
 hzsec = t2.tms_utime-t1.tms_utime;
 return(hzsec*1000/60);
}

struct tms timestamp,s1,s2;
int garbagetime;

suspendtimers()
{
 times(&s1);
}

restarttimers()
{
 times(&s2);
 garbagetime += s2.tms_utime-s1.tms_utime;
}

resettimers()
{
 garbagetime = 0;
 s2.tms_utime = s1.tms_utime = 0;
 t2.tms_utime = t1.tms_utime = 0;
 collected = collectedfrom = minorcollections = majorcollections = ML_INT(0);
}


timer()
{
 times(&timestamp);
 g_usec = garbagetime % 60 * 1000000 / 60 * 2 + 1;
 g_sec = garbagetime / 60 * 2 + 1;
 t_usec = timestamp.tms_utime % 60 * 1000000 / 60 * 2 + 1;
 t_sec = timestamp.tms_utime / 60 * 2 + 1;
}

#endif	/* V9 */

int uniq(arg) int arg;
{int q, *p;
#define BASE(ptr) (((int**)(ptr))[0])
#define INDEX(ptr) ((((int*)(ptr))[1]-1)>>1)
#define NEXTP(ptr) (((int*)(ptr))[2])
 for(q=arg;q!=1;q=NEXTP(q))
    if ( INDEX(q) == -1)
      {((int*)q)[-1]= BASE(q)[-1];
       ((int**)q)[0][-1] = 0;
      }
 for(q=arg;q!=1;q=NEXTP(q))
    if ( BASE(q)[-1] && INDEX(q)>=0 && BASE(q)[INDEX(q)])
      {((int*)q)[-1]= BASE(q)[INDEX(q)];
       BASE(q)[INDEX(q)] = 0;
      }
 p= &arg;
 while (*p!=1)
    if (((int**)*p)[0][(((int*)*p)[1]-1)/2])
          *p = NEXTP(*p);
    else  {BASE(*p)[INDEX(*p)] = ((int*)*p)[-1];
           ((int*)*p)[-1] = 3*16+1;
           p = &NEXTP(*p);
          }

 return arg;
}

#ifdef ADVICE

static char mallocbuf[0x10000], *mallocptr=mallocbuf;

char *malloc(n) int n;
{char *p;
 n = (n+3) & ~3;
 if (mallocptr+n > mallocbuf+0x10000) die ("Too much malloc.");
 p=mallocptr;
 mallocptr+=n;
 return p;
}

char *free(){}

char *realloc(){
    die ("Didn't expect realloc.");
}

char *calloc(n,m) int n,m; 
{char *p = malloc(n*m); int i;
 for(i=0;i<n;i++) p[i]=0;
 return p;    
}

char *cfree() {}
#endif
