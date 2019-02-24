/* reloc for 5620's */

#include "stdio.h"
#include "filehdr.h"
#include "aouthdr.h"
#include "reloc.h"
#include "scnhdr.h"
#include "sys/types.h"
#include "sys/stat.h"
extern char *malloc();

int textorg;
struct stat stb;
char *buf;
struct scnhdr *ts, *ds, *bs;
struct filehdr *fhdr;
long swapb4();

#ifdef TEST
#define DEBUG(X,Y)	printf(X,Y)
#else
#define DEBUG(X,Y)
#endif

main(argc, argv)
char **argv;
{	int fd;
	struct aouthdr *ahdr;
	if(argc != 4) {
		fprintf(stderr, "usage: %s -b addr file\n",argv[0]);
		exit(1);
	}
	sscanf(argv[2], "%x", &textorg);
	fd = open(argv[3], 0);
	if(fd < 0) {
		perror(argv[3]);
		exit(1);
	}
	fstat(fd, &stb);
	if(stb.st_size <= sizeof(struct filehdr)) {
		perror("stdin");
		exit(1);
	}
	buf = malloc(stb.st_size);
	if(read(fd, buf, stb.st_size) != stb.st_size) {
		perror("read");
		exit(1);
	}
	fhdr = (struct filehdr *) buf;
	if(fhdr->f_magic != FBOMAGIC) {
		fprintf(stderr, "reloc: file magic was 0%o expected 0%o\n",
			fhdr->f_magic, FBOMAGIC);
		exit(1);
	}
	if(fhdr->f_nscns != 3) {
		fprintf(stderr, "reloc: file has %d sections instead of 3\n",
			fhdr->f_nscns);
		exit(1);
	}

	ts = (struct scnhdr *) (buf + sizeof(struct filehdr) + fhdr->f_opthdr);
	ds = ts + 1;	/* is this arithmetic ok? */
	bs = ds + 1;
	reloc(ts);
	reloc(ds);
	reloc(bs);
	fhdr->f_flags |= F_RELFLG;

	ahdr = (struct aouthdr *) (buf + sizeof(struct filehdr));
	ahdr->entry += textorg;
	ahdr->text_start += textorg;
	ahdr->data_start += textorg;
	ahdr->vstamp = 0;
	fhdr->f_symptr = fhdr->f_nsyms = 0;
	write(1, buf, stb.st_size);
	exit(0);
}

reloc(sp, org)
struct scnhdr *sp;
{	struct reloc *rp, *realrp, copyarea;
 	int i, j;
 	int space;
	unsigned int addr;
	struct syment *sym;
	unsigned char *from, *to, *foo, *saveto;
	realrp = (struct reloc *) (buf + sp->s_relptr);
	rp = &copyarea;
	memcpy(rp, realrp, RELSZ);
	for(i = 0; i < sp->s_nreloc; i++) {
		switch(rp->r_type) {
		default:
			fprintf(stderr, "reloc type 0%o unknown\n", rp->r_type);
			exit(1);
		case R_DIR32:
			DEBUG(" R_DIR32 (%d)",rp->r_type);
			to = (unsigned char *) (buf + sp->s_scnptr + rp->r_vaddr - sp->s_vaddr);
#ifdef old
			/* swap bytes on the way in... */
			addr = to[0];
			for(j = 1; j<=3; j++)
				addr = (addr << 8) | to[j];
			addr += textorg;
			/* swap bytes on the way out... */
			from = (unsigned char *)&addr;
			for(j = 0; j < 4; j++)
				*to++ = from[3-j];
			break;
#endif
			DEBUG(" to=%x, ",*to);
#ifdef vax
			from = (unsigned char *) &space;
			saveto = to;
			for(j = 3; j >= 0 ; j--)
				*from++ = to[j];
			space += textorg;
			from = (unsigned char *) &space;
			to = saveto;
			for(j = 3; j >= 0 ; j--)
				to[j] = *from++;
#else
			from = (char *) &space;
			saveto = to;
			for(j = 0; j < 4; j++)
				*from++ = to[j];
			space += textorg;
			from = (char *) &space;
			to = saveto;
			for(j = 0; j < 4; j++)
				to[j] = *from++;
#endif vax
			DEBUG("space=%x\n",space);  
			break;

		case R_DIR32S:
			DEBUG(" R_DIR32S (%d)",rp->r_type); 
			to = (unsigned char *) (buf + sp->s_scnptr + rp->r_vaddr - sp->s_vaddr);
#ifdef wannadoitfastonavax
			/* alignment and byte order? */ /* vax byte order */
			addr = *(int *)to;
		/*	addr += textorg;	*/
			addr = swapb4( swapb4(addr,1) + textorg, 1);
			*(int *)to = addr;
#endif
			DEBUG(" to=%x, ",*to);
#ifdef vax
			from = (unsigned char *) &space;
			saveto = to;
			for(j = 0; j < 4; j++)
				*from++ = to[j];
			space += textorg;
			from = (unsigned char *) &space;
			to = saveto;
			for(j = 0; j < 4; j++)
				to[j] = *from++;
#else
			from = (char *) &space;
			saveto = to;
			for(j = 3; j >= 0 ; j--)
				*from++ = to[j];
			space += textorg;
			from = (char *) &space;
			to = saveto;
			for(j = 3; j >= 0 ; j--)
				to[j] = *from++;
#endif
			DEBUG("space=%x\n",space);  
		}
		/* Alignment needed for (structure) pointers on some hosts */
		realrp = (struct reloc *) ((char *)realrp +  RELSZ);
		memcpy(rp, realrp, RELSZ);
	}
	sp->s_vaddr += textorg;
	sp->s_paddr += textorg;
	sp->s_relptr = sp->s_lnnoptr = 0;
	sp->s_nreloc = sp->s_nlnno = 0;
}
