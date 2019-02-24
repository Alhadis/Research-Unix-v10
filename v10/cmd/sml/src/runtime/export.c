/* Copyright 1989 by AT&T Bell Laboratories */
#include "tags.h"
#ifdef NeXT
#include <sys/loader.h>
#else
#include <a.out.h>
#endif NeXT

#define CEIL(x,quantum) ((((int)(x))+(quantum)-1)&~((quantum)-1))

#ifdef NS32
#define N_DATADDR(x)	0x400000
#endif NS32

#ifndef N_DATADDR
#define N_DATADDR(x)  CEIL((x).a_text,getpagesize())
#endif

#ifndef N_TXTADDR
#ifdef NeXT
#include <machine/vm_param.h>
#ifdef USRTEXT
#define N_TXTADDR(x)  USRTEXT
#endif USRTEXT
#endif
#ifdef VAX
#define N_TXTADDR(x)  0
#endif
#ifdef NS32
#define N_TXTADDR(x)  0
#define exec aouthdr
#define a_magic		magic
#define	a_text		tsize
#define a_data		dsize
#define	a_bss		bsize
#define	a_entry		entry
#define	a_trsize	text_start
#define	a_drsize	data_start
#endif NS32
#ifndef sony_news
#ifdef M68
#define N_TXTADDR(x)  getpagesize()
#endif
#else sony_news
#define N_TXTADDR(x)  0
#endif sony_news
#ifdef SPARC
#include <machine/vmparam.h>
#define N_TXTADDR(x)  USRTEXT
#endif
#endif


 
/* Garbage collection is already done.
 * Data to be saved is:
 *  0 -> ceil(etext)		 text
 *  ceil(etext) -> arenabase	 data
 *  arenabase -> old_high	 heap
 *  
 *  > set a_entry as address of start procedure
 */

extern int etext;   /* &etext is just beyond the end of the text segment */
extern int old_high;

static int textstart,datastart;

extern int startptr;

#ifdef V9
getpagesize(){return 1024;}
#endif

#ifndef NeXT
export (filid)
    int filid;
{
    int bytcount;
   static struct exec E;  /* make it static so all fields=0 */

#if SUN3 || SPARC
    E.a_magic = NMAGIC;
#else
#ifdef NS32
    E.a_magic = NS32GMAGIC;
#else
    E.a_magic = ZMAGIC;
#endif
#endif
#ifdef M68
#ifndef sony_news
    E.a_machtype = 2;	/* M_68020 */
#endif sony_news
    E.a_machtype = M_68020; /* M_68020 */
#endif
#ifdef SPARC
    E.a_toolversion = 1;
    E.a_machtype = M_SPARC;
#endif

    textstart = N_TXTADDR(E);
    E.a_text = (int) CEIL(((int)&etext),getpagesize())-textstart;
    datastart = N_DATADDR(E);
    E.a_bss = 0;
#ifndef NS32
    E.a_syms = 0;
#endif NS32
    E.a_entry = startptr;
    E.a_trsize = 0;
    E.a_drsize = 0;
    E.a_data = CEIL(old_high-datastart, getpagesize());

    filid >>= 1;
    fchmod(filid,0755);
#ifdef NS32
    coff_cough(filid,&bytcount,E.a_text,E.a_data,E.a_entry);
#else
    bulletproofWrite(filid,&E,sizeof(E));
    bytcount = sizeof(E);
#endif NS32
#if VAX || NS32
    {int i, nzeros = getpagesize()-sizeof(E);
	char zeros[1024];
        for(i=0;i<nzeros;i++) zeros[i]=0;
        bulletproofWrite(filid,zeros,nzeros);

    }
#endif
#ifdef sony_news
    {int i, nzeros = getpagesize()-bytcount;
     char zeros[4096];
     for(i=0;i<nzeros;i++) zeros[i]=0;
     bulletproofWrite(filid,zeros,nzeros);
    }
#endif sony_news
    bulletproofWrite(filid,textstart,E.a_text);
    bulletproofWrite(filid,datastart,E.a_data);
}
#ifdef NS32
coff_cough(fd,countp,tsize,dsize,entry)
int fd, *countp, tsize, dsize, entry;
{
static struct filehdr fhdr;
static struct aouthdr ahdr;
static struct scnhdr thdr;
static struct scnhdr dhdr;
static struct scnhdr bhdr;
int allhdrsize = sizeof(fhdr) + sizeof(ahdr) + 3*sizeof(thdr);
int pagesize = getpagesize();

fhdr.f_magic = NS32GMAGIC;
fhdr.f_nscns = 3;
fhdr.f_timdat = /* don't care */ 0;
fhdr.f_symptr = /* null? */ 0;
fhdr.f_nsyms = 0;
fhdr.f_opthdr = sizeof(struct aouthdr);
fhdr.f_flags = F_RELFLG|F_EXEC|F_LNNO|F_LSYMS|F_AR32WR;

ahdr.magic = /* OOPS */ 0x010b;
ahdr.vstamp = /* don't care */ 0;
ahdr.tsize = tsize;
ahdr.dsize = dsize;
ahdr.bsize = 0;
ahdr.msize = /* OOPS */ 0x10;
ahdr.mod_start = /* OOPS */ 0x20;
ahdr.entry = entry;
ahdr.text_start = 0;
ahdr.data_start = 0x400000;
ahdr.entry_mod = /* unused? */ 0;
ahdr.flags = U_SYS_42|U_AL_4096;

strncpy(thdr.s_name,_TEXT,8);
thdr.s_paddr = thdr.s_vaddr = ahdr.data_start;
thdr.s_size = ahdr.tsize;
thdr.s_scnptr = CEIL(allhdrsize,pagesize);
thdr.s_relptr = /* null? */ 0;
thdr.s_lnnoptr = /* null? */ 0;
thdr.s_nreloc = 0;
thdr.s_nlnno = 0;
thdr.s_flags = STYP_TEXT;
thdr.s_symptr = /* null? */ 0;
thdr.s_modno = /* OOPS */ 0;
thdr.s_pad = /* don't care */ 0;

strncpy(dhdr.s_name,_DATA,8);
dhdr.s_paddr = dhdr.s_vaddr = ahdr.data_start;
dhdr.s_size = ahdr.dsize;
dhdr.s_scnptr = thdr.s_scnptr + CEIL(thdr.s_size,pagesize);
dhdr.s_relptr = /* null? */ 0;
dhdr.s_lnnoptr = /* null? */ 0;
dhdr.s_nreloc = 0;
dhdr.s_nlnno = 0;
dhdr.s_flags = STYP_DATA;
dhdr.s_symptr = /* null? */ 0;
dhdr.s_modno = /* OOPS */ 0;
dhdr.s_pad = /* don't care */ 0;

strncpy(bhdr.s_name,_BSS,8);
bhdr.s_paddr = bhdr.s_vaddr = ahdr.data_start + ahdr.dsize;
bhdr.s_size = /* none */ 0;
bhdr.s_scnptr = /* null */ 0;
bhdr.s_relptr = /* null? */ 0;
bhdr.s_lnnoptr = /* null? */ 0;
bhdr.s_nreloc = 0;
bhdr.s_nlnno = 0;
bhdr.s_flags = STYP_BSS;
bhdr.s_symptr = /* null? */ 0;
bhdr.s_modno = /* OOPS */ 0;
bhdr.s_pad = /* don't care */ 0;
 bulletproofWrite(fd,&fhdr,sizeof(fhdr));
 bulletproofWrite(fd,&ahdr,sizeof(ahdr));
 bulletproofWrite(fd,&thdr,sizeof(thdr));
 bulletproofWrite(fd,&dhdr,sizeof(dhdr));
 bulletproofWrite(fd,&bhdr,sizeof(bhdr));
 *countp = allhdrsize;
}
#endif NS32
#else NeXT
extern int mach_maplimit;
export(filid) int filid;
{static struct mach_header E;  /* make it static so all fields=0 */
static struct segment_command tcmd;
static struct section tsectn;
static struct segment_command dcmd;
static struct section dsectn;
static struct section bsectn;
static struct thread_command uthr;
static unsigned long thflavor;
static unsigned long thcount;
static struct NeXT_thread_state_regs ntregs;
static unsigned int hdrsize;
int datasize, bsssize;

E.magic = MH_MAGIC;
E.cputype = CPU_TYPE_MC68030;
E.cpusubtype = CPU_SUBTYPE_NeXT;
E.filetype = MH_EXECUTE;
E.ncmds = 3;
E.sizeofcmds = sizeof(tcmd) + sizeof(tsectn)
	+ sizeof(dcmd) + sizeof(dsectn) + sizeof(bsectn)
	+ sizeof(uthr) + sizeof(thflavor) + sizeof(thcount) + sizeof(ntregs);
E.flags = MH_NOUNDEFS;

hdrsize = E.sizeofcmds + sizeof(E);
 textstart = N_TXTADDR(E);


tcmd.cmd = LC_SEGMENT;
tcmd.cmdsize = sizeof(tcmd) + sizeof(tsectn);
strcpy(tcmd.segname,SEG_TEXT);
tcmd.vmaddr = textstart;
tcmd.vmsize = (int) CEIL(((int)&etext),getpagesize())-textstart;
tcmd.fileoff = 0;
tcmd.filesize = tcmd.vmsize;
tcmd.maxprot = VM_PROT_ALL;
tcmd.initprot = VM_PROT_READ|VM_PROT_EXECUTE;
tcmd.nsects = 1;
tcmd.flags = 0;

strcpy(tsectn.sectname,SECT_TEXT);
strcpy(tsectn.segname,tcmd.segname);
tsectn.addr = tcmd.vmaddr + hdrsize;
tsectn.size = tcmd.vmsize - hdrsize;
tsectn.offset = hdrsize;
tsectn.align = 2;
tsectn.reloff = 0;
tsectn.nreloc = 0;
tsectn.flags = 0;
tsectn.reserved1 = tsectn.reserved2 = 0;


dcmd.cmd = LC_SEGMENT;
dcmd.cmdsize = sizeof(dcmd) + sizeof(dsectn) + sizeof(bsectn);
strcpy(dcmd.segname,SEG_DATA);
dcmd.vmaddr = tcmd.vmaddr + tcmd.vmsize;
datasize = CEIL(old_high-dcmd.vmaddr, getpagesize());
bsssize = mach_maplimit-old_high;
dcmd.vmsize = bsssize+datasize;
dcmd.fileoff = tcmd.fileoff + tcmd.filesize;
dcmd.filesize = datasize;
dcmd.maxprot = VM_PROT_ALL;
dcmd.initprot = VM_PROT_ALL;
dcmd.nsects = 2;
dcmd.flags = 0;

strcpy(dsectn.sectname,SECT_DATA);
strcpy(dsectn.segname,dcmd.segname);
dsectn.addr = dcmd.vmaddr;
dsectn.size = datasize;
dsectn.offset = dcmd.fileoff;
dsectn.align = 2;
dsectn.reloff = 0;
dsectn.nreloc = 0;
dsectn.flags = 0;
dsectn.reserved1 = dsectn.reserved2 = 0;

strcpy(bsectn.sectname,SECT_BSS);
strcpy(bsectn.segname,dcmd.segname);
bsectn.addr = dsectn.addr + dsectn.size;
bsectn.size = bsssize;
bsectn.offset = 0;
bsectn.align = 2;
bsectn.reloff = 0;
bsectn.nreloc = 0;
bsectn.flags = S_ZEROFILL;
bsectn.reserved1 = bsectn.reserved2 = 0;

uthr.cmd = LC_UNIXTHREAD;
uthr.cmdsize = sizeof(uthr) + sizeof(thflavor) + sizeof(thcount) + sizeof(ntregs);

thflavor = NeXT_THREAD_STATE_REGS;

thcount = NeXT_THREAD_STATE_REGS_COUNT;

ntregs.areg[0] = 0xfeadface;
ntregs.pc = startptr;

 filid >>= 1;
 fchmod(filid,0755);
 bulletproofWrite(filid,&E,sizeof(E));
 bulletproofWrite(filid,&tcmd,sizeof(tcmd));
 bulletproofWrite(filid,&tsectn,sizeof(tsectn));
 bulletproofWrite(filid,&dcmd,sizeof(dcmd));
 bulletproofWrite(filid,&dsectn,sizeof(dsectn));
 bulletproofWrite(filid,&bsectn,sizeof(bsectn));
 bulletproofWrite(filid,&uthr,sizeof(uthr));
 bulletproofWrite(filid,&thflavor,sizeof(thflavor));
 bulletproofWrite(filid,&thcount,sizeof(thcount));
 bulletproofWrite(filid,&ntregs,sizeof(ntregs));
 bulletproofWrite(filid,tsectn.addr,tsectn.size);
 bulletproofWrite(filid,dsectn.addr,dsectn.size);
}
#endif NeXT


/* NICK: A Bullet-proof write to retry on NFS systems */
/* JHR: Added retry for NFS timeout errors. */
#include <errno.h>
bulletproofWrite(fid, buf, total)
int fid;
char *buf;
int total;
{
   int bytesWritten = 0;
#ifdef M68
   int retries = 0;
#endif
   int i;
   do
      {	 i = write(fid, buf, total-bytesWritten);
#ifdef M68
#ifndef sony_news
     if (i < 0) {
	if (errno == ETIMEDOUT) {
	   /* NFS timeout error, so try again. */
	   if (retries++ > 5)
	      die("export, NFS timeout");
	   chatting("[Write timeout, retrying]\n");
	   continue;
	}
	else die("export");
     }
     else retries = 0;
#else sony_news
	 if (i < 0) die("export");
#endif sony_news
#else
     if (i < 0) die("export");
#endif
     bytesWritten += i;
     buf += i;
     if (bytesWritten < total)
	chatting("[Write incomplete (%d/%d), retrying]\n",
	     bytesWritten, total);
      }
   while (bytesWritten < total);
}
