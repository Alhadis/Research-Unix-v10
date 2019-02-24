/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// encode.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 06:33:03 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * ENCODE BASIC COMMAND
 * mod,J.P.Hawkins,17-JUL-79, add "else" and "endif" commands
 */
/*   "@(#) encode.c:  V 1.3  11/29/81" */
#include "bas.h"
/*
 *
 * //////// BASIC INTERPRETER COMMAND TABLE ////////
 *
 */
int	delete();	/* delete specified line number(s) */
int	brake();	/* break out of a for loop */
int	cont();
int	dim();		/* alloc specified data array */
int	__end();	/* define end of program */
int	expunge();	/* expunge variables */
int	_for();		/* initialize and/or test "for" loop */
int	gosub();	/* goto subroutine, return to next statement */
int	call();		/* overlay and gosub */
int	__goto();	/* goto specified statement number */
int	__if();		/* if/then/goto test */
int	__input();	/* accept input from stdio dev */
int	new();		/* clean out BASIC program txtbuf */
int	let();		/* execute assignment */
int	list();		/* reverse translate, then list */
int	next();		/* terminate last "for" statement */
int	pause();	/* wait for spacebar or panel pause button */
int	__print();	/* print string and/or variables */
int	_random();	/* plant random "seed" for "rnd" cmd */
int	__read();	/* initialized data */
int	no_op();	/* ignore this statemtnt */
int	restore();	/* restore initialized data area pointer */
int	__return();	/* return from subroutine called by gosub */
int	sub();		/* substitute command for editing */
int	run();		/* run or load and run a BASIC core image */
int	sing();		/* single step execution */
int	con();		/* continue from single step */
int	stop();		/* halt program execution and re-init */
int	minus();	/* print previous program line */
int	reseq();	/* resequence line numbers */
int	mov();		/* move a group of lines */
int	autnum();		/* start auto line numbering */
int	save();		/*save basic program full ascii text form*/
int	f();		/* print current filename */
int	load();		/*read in basic file in full ascii text form*/
int	old();		/* clear user core, the read in BASIC prog */
int	corleft();	/* returns amount of core bytes remaining in decimal */
int	undo();		/* undoes the last substite command */
int	_common();	/* preserve variables for next "run" */
int	on();		/* on goto, on gosub */
int	goaway();	/* "bye" command exits the interpreter */
int	openo();	/* OPEN file(s) for write */
int	openi();	/* OPEN file(s) for reading */
int	__seek();	/* random access */
int	rewin();	/* Rewind to beginning of file */
int	append();	/* Open file for append, create if non-exist */
int	_closef();	/* CLOSE FILE */
int	clall();	/* Close all open files */
int	iprintf();	/* interpretive printf */
int	_else();	/* ELSE command for structured "if" */
int	_endif();	/* Structured "if" terminator */
int	sysls();	/* direct call to ls */
int	sysrm();	/* direct call to rm */

#ifndef	LSX
int	bsys();		/* "!" command for system calls */
#endif
#ifdef	TEST
int	buscmd();	/* IEEE BUS COMMAND */
int	buspr();	/* print to IEEE BUS */
int	ps();		/* set power supply */
int	relay();	/* set relays command */
int	bscan();	/* set scanner command */
int	bdelay();	/* 1/60 sec delay command */
int	bdvminit();	/* dvm initializer */
int	bdvms();	/* dvm setup command */
int	bhprintf();	/* strip printer driver */
int	lodset();	/* load setting driver */
#endif
#ifdef LSX
int	chdir();
int	lsdir();
int	pwd();
#endif
/*
 *
 * This is the BASIC interpreter command string and routine
 * dispatch table.
 * This table is used for forward translation of a BASIC text
 * file to it's core image form and is also used in the reverse
 * translation process.
 * In loadin the basic text file (.b) the table is searched
 * for each command encountered in the file and is used to replace
 * the command with a "PSEUDO CODE" or the relative position of the
 * code in the table. This process also occurs when an input line is typed
 * before it is stored for a run.
 * When a run command is executed the table need-NOT be searched
 * as the offset is already "compiled" in the code.
 * The basic execution routine is then executed by an indirect,
 * indexed jump through this table.
 * The format of a line in a BASIC core image is as follows:
 *	first two bytes represent the integer value of the line number.
 *	Second two bytes represent the PSEUDO code or table offset.
 *	Then the rest of the line is in it's original TEXT form,
 *		terminated by a line-feed (NO-NULL)
 *
 *
 */

/*
 * each entry contains the text for the BASIC command
 * in question and the address of the routine which sevices it
 */
struct COMMAND cmdtbl[] = {
	{"common",	_common},
	{"on",		on},
	{"com",		_common},
	{"data",	no_op},
	{"delete",	delete},
	{"break",	brake},
	{"continue",	cont},
	{"del",		delete},
	{"dim",		dim},
	{"end",		__end},
	{"bye",		goaway},
	{"q",		goaway},
	{"expunge",	expunge},
	{"for",		_for},
	{"gosub",	gosub},
	{"call",	call},
	{"goto",	__goto},
	{"go to",	__goto},
	{"if",		__if},
	{"input",	__input},
	{"new",		new},
	{"let",		let},
	{"\010",	let},
	{"list",	list},
	{"l",		list},
	{"n",		list},
	{"next",	next},
	{"pause",	pause},
	{"pr",		__print},
	{"print",	__print},
	{"randomize",	_random},
	{"read",	__read},
	{"rem",		no_op},
	{"restore",	restore},
	{"return",	__return},
	{"s",		sub},	/* substitute command */
	{"undo",	undo},
	{"run",		run},
	{"sing",	sing},
	{"con",		con},
	{"stop",	stop},
	{"-",		minus},
	{"reseq",	reseq},
	{"mov",		mov},
	{"auto",	autnum},
	{"a",		autnum},
	{"save",	save},
	{"w",		save},
	{"f",		f},
	{"load",	load},
	{"old",		old},
	{"size",	corleft},
	{"openo",	openo},
	{"openi",	openi},
	{"seek",	__seek},
	{"rewin",	rewin},
	{"close",	_closef},
	{"closeall",	clall},
	{"append",	append},
	{"printf",	iprintf},
	{"else",	_else},
	{"endif",	_endif},
	{"ls",		sysls},
	{"rm",		sysrm},
#ifndef	LSX
	{"!",		bsys},
#endif
#ifdef	TEST
	{"cmd",		buscmd},
	{"buspr",	buspr},
	{"ps",		ps},
	{"relay",	relay},
	{"dvminit",	bdvminit},
	{"scan",	bscan},
	{"delay",	bdelay},
	{"dvms",	bdvms},
	{"hprintf",	bhprintf},
	{"lodset",	lodset},
#endif
#ifdef LSX
	{"cd",		chdir},
	{"ls",		lsdir},
	{"pwd",		pwd},
#endif
	{0,		0}	/* null terminator */
	};

goaway()
{
	clall();
	exit(0);
}
/*
 *
 * calling format:
 *	code = encode(string);
 *
 *	where: code = integer returned as opcode (table offset)
 *			-1 (minus one) returned if search failed
 *	       string = pointer to string containing code text
 *			to be translated or encoded (null terminated)
 */

encode(s)
char	s[];
{
	register int i;		/* index reg. for expediency */
	/*
	 * compare each string in table with s
	 * when match is found, return index (OPCODE)
	 * if end-of-table (null) encountered return -1
	 */
	for(i=0; cmdtbl[i].cmdtxt != 0 ; i++)
	{
		if(!(strcmp(s, cmdtbl[i].cmdtxt)))
			return(i);	/* we found it! */
	}
	return(-1);	/* oops not in this table, pal */
}
