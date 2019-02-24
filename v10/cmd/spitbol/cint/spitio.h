/*
/   This header file defines the I/O control blocks used by the
/   UN*X operating system interface for the Macro Spitbol compiler.
*/

/*
/   BLBLK - I/O buffer block
/
/   All buffered I/O is passed through a BFBLK.  This block is intentionally
/   kept non-relocatable, so that it can be freely moved around the heap by
/   the garbage collector.
*/

struct bfblk
{
    WORD	typ;		/*  type word				*/
    WORD	len;		/*  lenfth of bfblk			*/
    WORD	siz;		/*  size of buffer in bytes		*/
    WORD	rem;		/*  number of bytes remaining in buffer	*/
    WORD	off;		/*  offset to next position in buffer	*/
    char	buf[1];		/*  buffer ([1] is kludge for C)	*/
};

#define BFSIZE		(sizeof struct bfblk - 1)

/*
/   FCBLK - file control block
/
/   For every I/O association a FCBLK is created.  All subsequent I/O
/   operations are passed this block.
*/

struct fcblk
{
    WORD	typ;		/*  type word				*/
    WORD	len;		/*  length of fcblk			*/
    WORD	rsz;		/*  record size & mode			*/
    struct ioblk *iob;		/*  pointer to IOBLK			*/
};

#define FCSIZE		(sizeof struct fcblk)

/*
/   IOBLK - I/O control block
/
/   For every SPITBOL I/O channel there is one central IOBLK containing
/   information about the channel:  filename, file descriptor, IOBLK
/   pointer, etc.
*/

struct ioblk
{
    WORD	typ;		/*  type word				*/
    WORD	len;		/*  length of IOBLK			*/
    struct scblk *fnm;		/*  pointer to SCBLK holding filename	*/
    WORD	pid;		/*  process id for pipe			*/
    struct bfblk *buf;		/*  pointer to BFBLK			*/
    WORD	fdn;		/*  file descriptor number		*/
    WORD	flg;		/*  flags				*/
};

#define IOSIZE		(sizeof struct ioblk)

#define IO_INP		1
#define IO_OUP		2
#define IO_APP		4
#define IO_OPN		8
#define IO_EOF		16
#define IO_ERR		32
#define IO_SYS		64
#define IO_WRC		128
#define IO_PIP		256
#define IO_DED		512
#define IO_ILL		1024

#define RECSIZ		1024
#define BUFSIZ		1024
