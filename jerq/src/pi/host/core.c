#include "process.pub"
#include "frame.pri"
#include "symtab.pri"
#include "symbol.h"
#include "core.pri"
#include "master.pub"
SRCFILE("core.c")

void Cslfd.init(long l, double d) { flterr=0; flt=dbl=d; chr=sht=lng=l; }
Cslfd.Cslfd(long l)		  { init(l, 0.0); }	
Cslfd.Cslfd(double d)		  { init(0,   d); }
Cslfd.Cslfd(int i)		  { init(i, 0.0); }
Cslfd.Cslfd()			  { init(0, 0.0); }
char *Cslfd.floaterror()	  { return flterr; }

Core.Core(Process *p, Master *m)
{
	_process = p;
	_symtab = (m && m->core) ? m->core->_symtab : 0;
	corefd = stabfd = -1;
	_online = 0;
	behavs_problem = "?";
}

char *Core.docall(long,int)		{ return "docall";		}
long Core.apforcall(int)		{ return 0; }
long Core.returnregloc()		{ return 0; }
int Core.corefstat()			{ return fstat(corefd,&corestat); }
int Core.stabfstat()			{ return fstat(stabfd,&stabstat); }
Asm *Core.newAsm()			{ return 0;			}
Behavs Core.behavs()			{ return 0;			}
Cslfd *Core.peekcode(long l)		{ return peek(l);		}
CallStk *Core.callstack()		{ return 0;			}
Frame Core.frameabove(long)		{ Frame f; return f;		}
Process *Core.process()			{ return _process;		}
SymTab *Core.symtab()			{ return _symtab;		}
char *Core.special(char*,long)		{ return "special";		}
char *Core.destroy()			{ return "destroy";		}
char *Core.eventname()			{ return "eventname";		}
char *Core.laybpt(Trap*)		{ return "laybpt";		}
char *Core.liftbpt(Trap*)		{ return "liftbpt";		}
char *Core.open()			{ return "open";		}
char *Core.peekstring(long,char*)	{ return "peekstring";		}
char *Core.poke(long,long,int)		{ return "poke long";		}
char *Core.pokedbl(long,double,int)	{ return "poke double";	}
char *Core.procpath()			{ return _process->procpath;	}
char *Core.read(long,char*,int)		{ return "read";		}
char *Core.readcontrol()		{ return "readcontrol";		}
char *Core.regname(int)			{ return "regname";		}
char *Core.regpoke(int r,long l)	{ return poke(regloc(r), l, 4);	}
char *Core.reopen(char*,char*)		{ return "reopen";		}
char *Core.resources()			{ return "resources";		}
char *Core.run()			{ return "run";			}
char *Core.stabpath()			{ return _process->stabpath;	}
char *Core.popcallstack()		{ return "popcallstack";	}
char *Core.step(long,long)		{ return "step";		}
char *Core.stepprolog()			{ return 0;			}
char *Core.stop()			{ return "stop";		}
char *Core.write(long,char*,int)	{ return "write";		}
int Core.REG_AP()			{ return 0;			}
int Core.REG_FP()			{ return 0;			}
int Core.REG_PC()			{ return 0;			}
int Core.REG_SP()			{ return 0;			}
char *Core.specialop(char*)		{ return 0;			}
int Core.event()			{ return 0;			}
int Core.online()			{ return _online;		}
long Core.ap()			{ return regpeek(REG_AP());	}
long Core.fp()			{ return regpeek(REG_FP());	}
long Core.pc()			{ return regpeek(REG_PC());	}
long Core.regloc(int,int)		{ return 0;			}
long Core.regpeek(int r)		{ return peek(regloc(r))->lng;	}
long Core.saved(Frame*,int,int)		{ return 0;			}
long Core.sp()			{ return regpeek(REG_SP());	}

char *Core.problem()
{
	return sf("unanticipated problem: %s; mail tac", behavs_problem);
}

Cslfd *Core.peek(long loc, Cslfd *fail)
{
	static Cslfd *c;
	trace( "%d.peek(%d,%d)", this, loc, fail );
	if( fail != PEEKFAIL ) return fail;
	return c ? c : (c = new Cslfd);
}

char *BehavsName(Behavs b)
{
	switch(b){
	case BREAKED:		return "BREAKPOINT:";
	case ERRORED:		return "ERROR STATE:";
	case HALTED:		return "STOPPED:";
	case ACTIVE:		return "RUNNING:";
	case PENDING:		return "EVENT PENDING:";
	case INST_STEPPED:	return "INSTR STEPPED:";
	case STMT_STEPPED:	return "STMT STEPPED:";
	}
	return Name( "Behavs=%d", b);
}

void Core.close()
{
	trace( "%d.close()", this );	VOK;
	if( corefd>=0 ) ::close(corefd);
	if( stabfd>=0 ) ::close(stabfd);
	if( _symtab ) delete _symtab;
}

char *Core.blockmove(long s, long d, long ct)
{
	trace( "%d.blockmove(0x%X,0x%X,%d)", this, s, d, ct); OK("Core.blockmove");
	char *buf = new char[ct];
	char *error = read(s, buf, ct);
	if( !error ) error = write(d, buf, ct);
	delete buf;
	return error;
}

void Context.restore()
{
	trace( "%d.restore()", this );	VOK;
	error = "context restore error";
}

Context *Core.newContext()
{
	return new Context;
}

void Core.slavedriver(Core *sd)
{
	trace("%d.slavedriver()"); VOK;
	sd->_symtab->inherit = _symtab;
}
