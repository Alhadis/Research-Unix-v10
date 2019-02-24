/*	@(#)xec.c	1.8	*/
/*
 *
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */


#include	"defs.h"
#include	<errno.h>
#include	"sym.h"
#include	<sys/types.h>
#include	<sys/stat.h>

static int	parent;

/* ========	command execution	========*/


execute(argt, exec_link, errorflg, pf1, pf2)
struct trenod	*argt;
int	*pf1, *pf2;
{
	/*
	 * `stakbot' is preserved by this routine
	 */
	register struct trenod	*t;
	char		*sav = savstak();

	sigchk();
	if (!errorflg)
		flags &= ~errflg;

	if ((t = argt) && execbrk == 0)
	{
		register int	treeflgs;
		int 			type;
		register char	**com;
		int 			linked;
		int 			execflg;

		linked = exec_link >> 1;
		execflg = exec_link & 01;

		treeflgs = t->tretyp;
		type = treeflgs & COMMSK;

		switch (type)
		{
		case TFND:
			{
				struct fndnod	*f = (struct fndnod *)t;
				struct namnod	*n = lookup(f->fndnam);

				exitval = 0;

				if (special(n->namid))
					failed(n->namid, badfname, 0);
				if (!(n->namflg & N_ENVNAM))
					free_val(&n->namval);
	
				if (funcnt && f->fndval)
					f->fndval->tretyp++;

				n->namval.val = (char *)f->fndval;
				n->namval.flg = N_FUNCTN;
				if (flags & exportflg)
					attrib(n, N_EXPORT);
				break;
			}

		case TCOM:
			{
				char	*a1;
				int	argn, internal;
				struct argnod	*schain = gchain;
				struct ionod	*io = t->treio;
				struct namnod	*n;
				exitval = 0;

				gchain = 0;
				argn = getarg(t);
				com = scan(argn);
				a1 = com[1];
				gchain = schain;

				if((n = findnam(com[0])) && !(n->namval.flg & N_FUNCTN))
					n = 0;
				if ((internal = syslook(com[0], commands, no_commands)) || argn == 0)
					setlist(comptr(t)->comset, 0);

				if (argn && (flags&noexec) == 0)
				{		
					/* print command if execpr */

					if (flags & execpr)
						execprint(com);

					if (n)	/* function */
					{
						short index;

						funcnt++;
						index = initio(io, 1);
						pushargs(com);
						execute((struct trenod *)(n->namval.val), exec_link, errorflg, pf1, pf2);
						execbrk = 0;
						restore(index);
						popargs();
						funcnt--;

						break;
					}

					else if (internal)
					{
						short index;
						index = initio(io, (internal != SYSEXEC));

					    isbltin:
						switch (internal)			
						{
						case SYSBLTIN:
							if (--argn > 0)
							{
								com++;
								a1 = com[1];
								if (internal = syslook(com[0], commands, no_commands))
									goto isbltin;
								failed(*com, notbltin, 0);
							}
							break;
	
						case SYSDOT:	
							if (a1)	
							{	
								register int	f;	
				
								if ((f = pathopen(getpath(a1), a1)) < 0)	
									failed(a1, notfound, 0);	
								else	
									execexp(0, f);	
							}	
							break;	
				
						case SYSTIMES:	
							{	
								long int t[4];	
				
								times(t);	
								prt(t[2]);	
								prc_buff(SP);	
								prt(t[3]);	
								prc_buff(NL);	
							}	
							break;	
				
						case SYSEXIT:	
							flags |= forked;	/* force exit */	
							exitsh(a1 ? stoi(a1) : retval);
				
						case SYSNULL:	
							io = 0;	
							break;	
				
						case SYSCONT:	
							if (loopcnt)
							{
								execbrk = breakcnt = 1;
								if (a1)
									breakcnt = stoi(a1);
								if (breakcnt > loopcnt)
									breakcnt = loopcnt;
								else
									breakcnt = -breakcnt;
							}
							break;	

						case SYSBREAK:	
							if (loopcnt)
							{
								execbrk = breakcnt = 1;
								if (a1)
									breakcnt = stoi(a1);
								if (breakcnt > loopcnt)
									breakcnt = loopcnt;
							}
							break;	

						case SYSTRAP:	
							if (a1)	
							{	
								BOOL 	clear;	
		
								if ((clear = digit(*a1)) == 0)	
									++com;	
								while (*++com)	
								{	
									int	i;	
		
									if ((i = stoi(*com)) >= MAXTRAP || i < MINTRAP)	
										failed(*com, badtrap, 0);	
									else if (clear)	
										clrsig(i);	
									else	
									{	
										replace(&trapcom[i], a1);	
										if (*a1)	
											getsig(i);	
										else	
											ignsig(i);	
									}	
								}	
							}	
							else	/* print out current traps */	
							{	
								int	i;	
				
								for (i = 0; i < MAXTRAP; i++)	
								{	
									if (trapcom[i])	
									{	
										prs_buff("trap ");
										prs_buff(quotedstring(trapcom[i]));
										prc_buff(SP);
										prn_buff(i);	
										prc_buff(NL);	
									}	
								}	
							}	
							break;	
				
						case SYSEXEC:	
							com++;	
							ioset = 0;	
							io = 0;	
							if (a1 == 0)	
							{	
								break;	
							}	
		
/*						case SYSNEWGRP:		*/
							oldsigs();	
							execa(com);
							done();	
		
						case SYSCD:	
							if ((a1 && *a1) || (a1 == 0 && (a1 = homenod.namval.val)))	
							{	
								char *cdpath;	
		
								if ((cdpath = cdpnod.namval.val) == 0 ||	
								     *a1 == '/' ||	
								     cf(a1, ".") == 0 ||	
								     cf(a1, "..") == 0 ||	
								     (*a1 == '.' && (*(a1+1) == '/' || *(a1+1) == '.' && *(a1+2) == '/')))	
									cdpath = nullstr;
								dochdir(a1, cdpath);
							}	
							else 
							{
								if (a1)
									failed(a1, baddir, 0);	
								else
									error(nohome);
							}

							break;	
				
						case SYSSHFT:	
							{	
								int places;	
		
								places = a1 ? stoi(a1) : 1;	
		
								if ((dolc -= places) < 0)
								{
									dolc = 0;
									error(badshift);
								}
								else
									dolv += places;
							}				
		
							break;	
				
						case SYSWAIT:	
							await(a1 ? stoi(a1) : -1, 1);	
							break;	
				
						case SYSREAD:	
							rwait = 1;	
							exitval = a1? readvar(&com[1]) : 0;	
							rwait = 0;	
							break;	
			
						case SYSSET:	
							if (a1)	
							{	
								int	argc;	
		
								argc = options(argn, com);	
								if (argc > 1)	
									setargs(com + argn - argc);	
							}	
							else if (comptr(t)->comset == 0)	
							{
								/*	
								 * scan name chain and print	
								 */	
								namscan(printnam);
							}
							break;	
				
						case SYSXPORT:	
							{
								struct namnod 	*n;

								exitval = 0;
								if (a1)	
								{	
									while (*++com)	
									{
										n = lookup(*com);
										attrib(n, N_EXPORT);
									}
								}	
								else	
									namscan(printexp);
							}
							break;	
				
						case SYSEVAL:	
							if (a1)	
								execexp(a1, &com[2]);	
							break;	
		
						case SYSUMASK:	
							if (a1)	
							{ 	
								int c, i;	
		
								i = 0;	
								while ((c = *a1++) >= '0' && c <= '7')	
									i = (i << 3) + c - '0';	
								umask(i);	
							}	
							else	
							{	
								int i, j;	
		
								umask(i = umask(0));	
								prc_buff('0');	
								for (j = 6; j >= 0; j -= 3)	
									prc_buff(((i >> j) & 07) +'0');	
								prc_buff(NL);	
							}	
							break;	
		
						case SYSRETURN:
							if (funcnt == 0)
								error(badreturn);

							execbrk = 1;
							exitval = (a1 ? stoi(a1) : retval);
							break;
							
						case SYSWHATIS:
							exitval = 0;
							if (a1)
							{
								while (*++com)
									what_is(*com);
							}
							break;

						case SYSUNS:
							exitval = 0;
							if (a1)
							{
								while (*++com)
									unset_name(*com);
							}
							break;

						case SYSMEM:
							exitval = 0;
							chkmem();
							break;
						default:	
							prs_buff("unknown builtin\n");
						}	
					

						flushb();
						restore(index);
						chktrap();
						break;
					}
				}
				else if (t->treio == 0)
				{
					chktrap();
					break;
				}

			}

		case TFORK:
			exitval = 0;
			if (execflg && (treeflgs & (FAMP | FPOU)) == 0)
				parent = 0;
			else
			{
				int forkcnt = 1;

				if (treeflgs & (FAMP | FPOU))
				{
					link_iodocs(iotemp);
					linked = 1;
				}


				/*
				 * FORKLIM is the max period between forks -
				 * power of 2 usually.  Currently shell tries after
				 * 2,4,8,16, and 32 seconds and then quits
				 */
	
				while ((parent = fork()) == -1)
				{
					if ((forkcnt = (forkcnt * 2)) > FORKLIM)	/* 32 */
					{
						switch (errno)
						{
						case ENOMEM:
							error(noswap);
							break;
						default:
						case EAGAIN:
							error(nofork);
							break;
						}
					}
					sigchk();
					alarm(forkcnt);
					pause(); 
				}
			}
			if (parent)
			{
				/*
				 * This is the parent branch of fork;
				 * it may or may not wait for the child
				 */
				if (treeflgs & FPRS && flags & ttyflg)
				{
					prn(parent);
					newline();
				}
				if (treeflgs & FPCL)
					closepipe(pf1);
				if ((treeflgs & (FAMP | FPOU)) == 0)
					await(parent, 0);
				else if ((treeflgs & FAMP) == 0)
					post(parent);
				else
					assnum(&pcsadr, parent);
				chktrap();
				break;
			}
			else	/* this is the forked branch (child) of execute */
			{
				flags |= forked;
				fiotemp  = 0;

				if (linked == 1)
				{
					swap_iodoc_nm(iotemp);
					exec_link |= 06;
				}
				else if (linked == 0)
					iotemp = 0;

				postclr();
				settmp();
				/*
				 * Turn off INTR and QUIT if `FINT'
				 * Reset ramaining signals to parent
				 * except for those `lost' by trap
				 */
				oldsigs();
				if (treeflgs & FINT)
				{
					signal(SIGINT, 1);
					signal(SIGQUIT, 1);

#ifdef NICE
					nice(NICEVAL);
#endif

				}
				/*
				 * pipe in or out
				 */
				if (treeflgs & FPIN)
				{
					rename(pf1[INPIPE], 0);
					close(pf1[OTPIPE]);
				}
				if (treeflgs & FPOU)
				{
					close(pf2[INPIPE]);
					rename(pf2[OTPIPE], 1);
				}
				/*
				 * default std input for &
				 */
				if (treeflgs & FINT && ioset == 0)
					rename(chkopen(devnull), 0);
				/*
				 * io redirection
				 */
				initio(t->treio, 0);

				if (type != TCOM)
				{
					execute(forkptr(t)->forktre, exec_link | 01, errorflg);
				}
				else if (com[0] != ENDARGS)
				{
					eflag = 0;
					setlist(comptr(t)->comset, N_EXPORT);
					rmtemp(0);
					execa(com);
				}
				done();
			}

		case TPAR:
			execute(parptr(t)->partre, exec_link, errorflg);
			done();

		case TFIL:
			{
				int pv[2];

				chkpipe(pv);
				if (execute(lstptr(t)->lstlef, 0, errorflg, pf1, pv) == 0)
					execute(lstptr(t)->lstrit, exec_link, errorflg, pv, pf2);
				else
					closepipe(pv);
			}
			break;

		case TLST:
			execute(lstptr(t)->lstlef, 0, errorflg);
			execute(lstptr(t)->lstrit, exec_link, errorflg);
			break;

		case TAND:
			if (execute(lstptr(t)->lstlef, 0, 0) == 0)
				execute(lstptr(t)->lstrit, exec_link, errorflg);
			break;

		case TORF:
			if (execute(lstptr(t)->lstlef, 0, 0) != 0)
				execute(lstptr(t)->lstrit, exec_link, errorflg);
			break;

		case TFOR:
			{
				struct namnod *n = lookup(forptr(t)->fornam);
				char	**args;
				struct dolnod *argsav = 0;

				if (forptr(t)->forlst == 0)
				{
					args = dolv + 1;
					argsav = useargs();
				}
				else
				{
					struct argnod *schain = gchain;

					gchain = 0;
					trim((args = scan(getarg(forptr(t)->forlst)))[0]);
					gchain = schain;
				}
				loopcnt++;
				while (*args != ENDARGS && execbrk == 0)
				{
					assign(n, *args++);
					execute(forptr(t)->fortre, 0, errorflg);
					if (breakcnt < 0)
						execbrk = (++breakcnt != 0);
				}
				if (breakcnt > 0)
						execbrk = (--breakcnt != 0);

				loopcnt--;
				argfor = (struct dolnod *)freeargs(argsav);
			}
			break;

		case TWH:
		case TUN:
			{
				int	i = 0;

				loopcnt++;
				while (execbrk == 0 && (execute(whptr(t)->whtre, 0, 0) == 0) == (type == TWH))
				{
					if(flags & noexec)
						break;
					i = execute(whptr(t)->dotre, 0, errorflg);
					if (breakcnt < 0)
						execbrk = (++breakcnt != 0);
				}
				if (breakcnt > 0)
						execbrk = (--breakcnt != 0);

				loopcnt--;
				exitval = i;
			}
			break;

		case TIF:
			if (execute(ifptr(t)->iftre, 0, 0) == 0)
				execute(ifptr(t)->thtre, exec_link, errorflg);
			else if (ifptr(t)->eltre)
				execute(ifptr(t)->eltre, exec_link, errorflg);
			else
				exitval = 0;	/* force zero exit for if-then-fi */
			break;

		case TSW:
			{
				register char	*r = mactrim(swptr(t)->swarg);
				register struct regnod *regp;

				regp = swptr(t)->swlst;
				while (regp)
				{
					struct argnod *rex = regp->regptr;

					while (rex)
					{
						register char	*s;

						if (gmatch(r, s = macro(rex->argval)) || (trim(s), eq(r, s)))
						{
							execute(regp->regcom, 0, errorflg);
							regp = 0;
							break;
						}
						else
							rex = rex->argnxt;
					}
					if (regp)
						regp = regp->regnxt;
				}
			}
			break;
		}
		exitset();
	}
	sigchk();
	tdystak(sav);
	flags |= eflag;
	return(exitval);
}

dochdir(place, cdpath)
char *place, *cdpath;
{
	char *dir;	
	char bestplace[256];
	char tempdir[256];
	int score, bestscore=1000;
	char *spname();
	struct stat sb;
	
	do	
	{	
		dir = cdpath;	
		cdpath = catpath(cdpath,place);
		/* kludge: will call opendir, so must lock down the string */
		movstr(curstak(), tempdir);
		if (chdir(tempdir) < 0)
		{
			char *dir1;

			if (flags&ttyflg && (dir1=spname(tempdir, &score))
			  && stat(dir1, &sb)==0
			  && (sb.st_mode&S_IFMT)==S_IFDIR
			  && access(dir1, 1)==0)
			{
				if (score < bestscore)
				{
					movstr(dir1, bestplace);
					bestscore = score;
				}
			}
		}
		else
		{
			if (cf(nullstr, dir) &&	
				*dir != ':' &&	
				any('/', tempdir) &&	
				flags & prompt)	
			{	
				prs_buff(tempdir);	
				prc_buff(NL);
			}
			return;	
		}
	} while (cdpath);
	if (bestscore < 1000 && chdir(bestplace) >= 0) {
		prs_buff(bestplace);	
		prc_buff(NL);
		return;
	}
	failed(place, baddir, 0);
}

execexp(s, f)
char	*s;
int	f;
{
	struct fileblk	fb;

	push(&fb);
	if (s)
	{
		estabf(s);
		fb.feval = (char **)(f);
	}
	else if (f >= 0)
		initf(f);
	execute(cmd(NL, NLFLG | MTFLG), 0, (int)(flags & errflg));
	pop();
}

execprint(com)
	char **com;
{
	register int 	argn = 0;

	prs(execpmsg);
	
	while(com[argn] != ENDARGS)
	{
		prs(quotedstring(com[argn++]));
		blank();
	}

	newline();
}
