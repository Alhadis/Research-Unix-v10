/*#include	"../h/param.h"*/
/*#include	"../h/dir.h"*/
/*#include	"../h/user.h"*/
/*#include	"../h/conf.h"*/
/*#include	"../h/seg.h"*/

#include	<sys/param.h>
#include	<sys/dir.h>
#include	<sys/user.h>
#include	<sys/ubaddr.h>
#include	<sys/buf.h>
#include	<sys/conf.h>
#include	<sys/pte.h>
#include	<sys/proc.h>
/*#include	<sys/seg.h>*/

#include	<sys/camac.h> 

int camopen(), camclose(), camread(), camwrite(), camioctl();

extern struct ubaddr camaddr[];		/* defined in conf..*/
extern int camcnt;
extern caddr_t camcaddr[];

struct cdevsw camcdev = cdinit(camopen, camclose, camread, camwrite, camioctl);

/*#define	BASE	0766000	*/		/* start addr of Camac registers for VAX*/
/*#define	BASE	0166000		 old LSI start addr */
/*
 * camac registers
 */

int camdebug = 0;

/*	the following is to make shure that the driver occupies
	a contiguous space in memory for all valid camac addresses
	01400 equals 768 addresses or 384 word addresses
	the base address is then the one chosen for the camac controller
	usually 0766000 (the csr is at N(24)A(0), which is BASE+01400)
*/

struct camregs {
	unsigned short	na[24][16];
	unsigned short	csr;
	unsigned short	datahi;
	unsigned short	lamlo;
	unsigned short	lamhi;
	unsigned short	reg4;
	unsigned short	reg5;
	unsigned short	reg6;
	unsigned short	int0;
};
struct camregs	*regs;

unsigned long	*intvector;
unsigned short	*BASE;

camopen(d,f)
{
	register int dev;

	if(camdebug){ printf(" started camopen \n");}

	if((dev = minor(d)) >= camcnt) {
		u.u_error = ENODEV;
		return;
	}

	/* get the addresses for regs...*/
/*	if((regs = (struct camregs *)ubaddr(&camaddr[dev])) == 0
	   || ubbadaddr(camaddr[dev].ubno, (caddr_t)regs, sizeof(u_short))) {
		printf("cam%d absent\n", dev); 
		u.u_error = ENODEV;
		return;
	}*/

	/* get the address from the configuration */
	/* the following will assign the range of contiguous addresses
	   for the physical I/O bus addresses */

	regs = (struct camregs *) ubaddr(&camaddr[dev]);

	if(camdebug){
		printf("address: value(csr):%o %o %o %o \n",
		&regs->csr,&regs->datahi,&regs->lamlo,&regs->lamhi);
	}
/*	
	the PDP/11 does it absolut, BASE=0766000
*/
/*	regs = (struct camregs *) (BASE + 01400);*/

	BASE = (&regs->na[0][0]);/* calculate the BASE relative to CSR */

	if(camdebug){
		printf(" BASE: %o \n",BASE);
	}

	if(camdebug){
	printf("BASE as calculated:%o \n",BASE);
	printf("should be         :20016750000\n");
	}


	regs->csr = 0;		/* initialize csr */

	if(camdebug){
		printf(" csr initialized: %o at addr %o\n", regs->csr,&regs->csr);
		printf(" exit camopen \n");
	}
}

camclose()
{
	/* not used */
}

camread()
{
	/* not used */
}

camwrite()
{
	/* not used */
}

camioctl(dev, cmd, usr_buf)
caddr_t		usr_buf;
{
	extern int		copyin(), copyout();	
	struct module_info	buffer1;
	struct reg_info		buffer2;

	unsigned 		*Dev_addr(), datalo, datahi;
	unsigned short		*device;
	char			*cdevice;

	if(camdebug) {
		printf(" starting camioctl \n");
		printf(" dev= %o cmd= %d usr_buf= %o at addr %o \n",
			dev,cmd,usr_buf,&usr_buf);
	}


	switch(cmd) {

	case CAMREAD:
	case CAMWRITE:
	case CAMSET:

		if(camdebug) {
			printf(" 1st copyin in driver \n");
			printf(" usr_buf %o \n",usr_buf);
		}

		if(copyin(usr_buf, (caddr_t)&buffer1, sizeof(buffer1)))
			u.u_error = EFAULT;

		device = &regs->na[buffer1.N][buffer1.A];

		if(camdebug) {
			printf(" address of device: %o \n",device);
		}
		
		if(camdebug) { 
		printf(" camioctldriver: %d %d", buffer1.N,buffer1.A);}

		switch(cmd) {

		case CAMREAD:
		/* Turn off no read bit, set f-bits, lv others unchanged */
		if(camdebug){
			printf("started CAMREAD \n");
		}
			regs->csr = (regs->csr & ~04007) | (buffer1.F & 07);
		if(camdebug){
			printf("set regs->csr = %o \n",regs->csr);
		}

			datalo = regs->na[buffer1.N][buffer1.A];

		if(camdebug){
			printf("in camread(driver) datalo=%o \n",datalo);
		}
			datahi = regs->datahi;
			buffer1.data = (long)datahi << 16;/*left shift 16 bits*/
			buffer1.data += (long) datalo;
			break;

		case CAMWRITE:
			if(camdebug){
				printf(" camwrite(driver) start \n");
				printf(" contents of buffer1:\n");
				printf("buffer1.F %o\n",buffer1.F);
				printf("buffer1.N %d\n",buffer1.N);
				printf("buffer1.A %d\n",buffer1.A);
				printf("buffer1.data %o\n",buffer1.data);
				printf("buffer1.Q %o\n",buffer1.Q);
			}
		/* Turn off no read bit, set f-bits, lv others unchanged */
			regs->csr = (regs->csr & ~04007) | (buffer1.F & 07);

		if(camdebug){
			printf("camwrite set regs->csr = %o \n",regs->csr);
			printf("put the buffer into the registers \n");
			printf("buffer1.data %o at %o", 
				buffer1.data,&buffer1.data);
		}
			regs->datahi = (buffer1.data >> 16);
					/*rightshift 16 bits*/

		if(camdebug){
			printf(" datahi= %o \n", regs->datahi);
			printf(" writing to address %o \n", device);
		}
			
			regs->na[buffer1.N][buffer1.A] = buffer1.data & 0177777;

		if(camdebug){
			printf(" written to address %o\n",device);
		}
			break;
		
		case CAMSET:
			regs->csr &= ~04000;	/* turn on "no-read" bit */
			cdevice = (char*)device; /* Cntrl ops need "MOVB"*/
			*cdevice = buffer1.F;
			if(camdebug){
			printf(" cdevice= %o at address= %o \n",cdevice,&cdevice);
			}
			regs->csr &= ~04000;	/* turn off no read bit */
			buffer1.Q = (regs->csr & 0200) >> 7;
			break;
		}
		
		buffer1.Q = (regs->csr & 0200) >> 7;
		if(copyout((caddr_t)&buffer1, usr_buf, sizeof(buffer1)))
			u.u_error = EFAULT;
			
		break;

	case CSRREAD:
	case LAMLOREAD:
	case LAMHIREAD:
	case VCTREAD:

		switch(cmd) {

		case CSRREAD:	buffer2.reg = regs->csr;
				break;

		case LAMLOREAD:	buffer2.reg = regs->lamlo;
				break;

		case LAMHIREAD:	buffer2.reg = regs->lamhi;
				break;

		case VCTREAD:	buffer2.reg = regs->int0;
				break;
		}

		if(copyout((caddr_t)&buffer2, usr_buf, sizeof(buffer2)))
			u.u_error = EFAULT;
		break;

	case CSRWRITE:
		if(copyin(usr_buf, (caddr_t)&buffer2, sizeof(buffer2)))
			u.u_error = EFAULT;

		regs->csr = buffer2.reg;
		break;
	}
}

cam0int()
{
	/* not used */
}
