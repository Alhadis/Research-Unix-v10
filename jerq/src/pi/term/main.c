#include "term.h"
#include "../protocol.h"

Proc *UserInterface;

typedef struct Handler {
	int	pkt;
	PFI	handler;
} Handler;

Version(); Spawner(); Spawnee(); Jps(); Debug(); Reloc(); Control();
Read(); Write(); String(); Graphic(); LiftBpt(); LayBpt(); CoreP();
Alloc(); Free(); Call();

Handler Handlers[] = {
	{ (int)	CG_BITMAP,	Graphic	},
	{ (int)	CG_POINT,	Graphic	},
	{ (int)	CG_RECT,	Graphic	},
	{ (int)	CG_TEXTURE,	Graphic	},

	{ (int)	CP_STACK,	CoreP	},
	{ (int)	CP_CONTROL,	CoreP	},
	{ (int)	CP_FLASH,	CoreP	},
	{ (int)	CP_REGLOC,	CoreP	},
	{ (int)	CP_RUN,	CoreP	},
	{ (int)	CP_STEP,	CoreP	},
	{ (int)	CP_STOP,	CoreP	},

	{ (int)	C_DEBUG,	Debug	},
	{ (int)	C_JPS,	Jps	},
	{ (int)	C_LAYBPT,	LayBpt	},
	{ (int)	C_LIFTBPT,	LiftBpt	},
	{ (int)	C_READ,	Read	},
	{ (int)	C_SPER,	Spawner	},
	{ (int)	C_SPEE,	Spawnee },
	{ (int)	C_STRING,	String	},
	{ (int)	C_VERSION,	Version },
	{ (int)	C_WRITE,	Write	},

	{ (int)	CF_ALLOC,	Alloc	},
	{ (int)	CF_FREE,	Free	},
	{ (int)	CF_CALL,	Call	},
	{ (int)	0,		0	}
};

void RCVServe()
{
	register pkt;
	register Handler *h;
	register PFI f;

	FlushRemote();
	while( own()&RCV ){
		pkt =  GetRemote();
		journal( "RCVServe",  "%x", pkt );
		for( f = 0, h = Handlers; !f && h->pkt; ++h )
			if( h->pkt == pkt )
				f = h->handler;
		assert(f);
		(*f)(pkt);
		FlushRemote();
	}
}

main()
{
	request(MOUSE|SEND|RCV);
	cursswitch(Cursor);
	sleep( 60 );
	FindProcTab();
	for( ;; ){
		RCVServe();
		DIRTYServe();
		MOUSEServe();
		wait(RCV|MOUSE);
	}
}

char ShellCmd[256];

SendCmd()	/* wait for shell prompt and send shell command */
{
	journal( "SendCmd", "%s", ShellCmd );
	PiTexture(P);
	while( !(own()&RCV) ) sleep(60);
	while(   own()&RCV  ) rcvchar();
	sendnchars(strlen(ShellCmd), ShellCmd);
	sendchar('\n');
	exit();
}

#define	C_NEW	2

long SpawnerPid;
Spawner() /* create layer and host process with shell cmd from current host */
{
	register Proc *p;

	UserInterface = 0;
	SpawnerPid = RcvLong();
	RcvString(ShellCmd);
	PutTextf( "pi.m loaded by pid=%d\nspawn: %s\n", SpawnerPid, ShellCmd );
	p = newproc(SendCmd);
	if( !p ) return 0;
	p->rect = P->rect;
	p->layer = newlayer(P->layer->rect);
	assert( p->layer );
	muxnewwind(p, C_NEW);
	UserInterface = p;
}

long SpawneePid;
Spawnee()		/* new host process spawned ok  */
{
	SpawneePid = RcvLong();
	PutTextf( "pi.m terminal agent for pid=%d\n", SpawneePid );
}

long TermVersion = PI_VERSION;
long HostVersion;
Version()
{
	char comment[256];
	HostVersion = RcvLong();
	RcvString(comment);
	PutTextf( "%s\n", comment );
	if( HostVersion == TermVersion ) return;
	PutTextf( "Versions Host=%d Term=%d\n", HostVersion, TermVersion );
	assert(0);
}
