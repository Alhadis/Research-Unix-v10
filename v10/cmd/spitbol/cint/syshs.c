/*
/   syshs - host functions
*/

#include "osint.h"
#include "spitblks.h"

extern WORD	gblargc;
extern char	*gblargv[];
extern char	*uarg;
extern WORD	cmdcnt;

extern struct icblk	ticblk;
extern struct scblk	tscblk;

extern WORD	ic_type;
extern WORD	sc_type;

syshs()
{
	WORD	retval;
	register struct icblk *icp = WA (struct icblk *);
	register struct scblk *scp;

/*
/   If argument one is an integer ...
*/
	if ( icp->typ == ic_type )
	    switch( icp->val )
	    {

/*
/	    HOST( 0 ) returns the -u command line option argument
*/
	    case 0:
		if ( uarg )
		{
		    cpys2sc( uarg, 0, &tscblk, tscblk.len );
		    SET_XL( &tscblk );
		    return  EXIT_3;
		}
		else
		    return  EXIT_4;

/*
/	    HOST( 1, "command" ) executes "command"
*/
	    case 1:
		scp = XL( struct scblk * );
		if ( scp->typ == sc_type )
		{
		    if ( scp->len != 0 )
		        dosys( scp );
		    return  EXIT_4;
		}
		else
		    return  EXIT_1;

/*
/	    HOST( 2, n ) returns command line argument n
*/
	    case 2:
		icp = XL( struct icblk * );
		if ( icp->typ == ic_type )
		{
		    retval = arg2scb( icp->val, gblargc, gblargv, &tscblk );
		    if ( retval < 0 )
			return  EXIT_6;
		    if ( retval == 0 )
			return  EXIT_1;
		    SET_XL( &tscblk );
		    return  EXIT_3;
		}	     	
		else
		    return  EXIT_1;

/*
/	    HOST( 3 ) returns the command count
*/
	    case 3:
		if ( cmdcnt )
		{
		    ticblk.val = cmdcnt;
		    SET_XR( &ticblk );
		    return  EXIT_5;
		}
		else
		    return  EXIT_6;

/*
/	    HOST( 4, "env-var" ) returns the value of "env-var" from
/	    the environment.
*/
	    case 4:
		scp = XL( struct scblk * );
		if ( scp->typ == sc_type )
		{
		    if ( scp->len == 0 )
			return  EXIT_1;
		    if ( rdenv( scp, &tscblk ) < 0 )
			return  EXIT_6;
		    SET_XL( &tscblk );
		    return  EXIT_3;
		}
		else
		    return  EXIT_1;

/*
/	    Any other integer value is an error
*/
	    default:
		return  EXIT_1;
	    }		


/*
/    ... else just return the host string
*/
	else
	{
	    gethost( &tscblk, TSCBLK_LENGTH );
	    if ( tscblk.len == 0 )
		return  EXIT_4;
	    SET_XL( &tscblk );
	    return  EXIT_3;
	}

/*
/   Control SHOULD NEVER GET HERE!!
*/
	return  EXIT_1;
}
