#include "jerq.h"
#include "io.h"
#include "commands.h"

Point trackarc(arg1, arg2, arg3, arg4, arg5)
int arg1;
int arg2;
Point arg3;
Point arg4;
Point arg5;
{
	sendCommand(CCTRACKARC);
	sendInt(arg1);
	sendInt(arg2);
	sendPoint(arg3);
	sendPoint(arg4);
	sendPoint(arg5);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackcircle(arg1, arg2, arg3, arg4)
int arg1;
int arg2;
Point arg3;
int arg4;
{
	sendCommand(CCTRACKCIRCLE);
	sendInt(arg1);
	sendInt(arg2);
	sendPoint(arg3);
	sendInt(arg4);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackdisc(arg1, arg2, arg3, arg4)
int arg1;
int arg2;
Point arg3;
int arg4;
{
	sendCommand(CCTRACKDISC);
	sendInt(arg1);
	sendInt(arg2);
	sendPoint(arg3);
	sendInt(arg4);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackelarc(arg1, arg2, arg3, arg4, arg5, arg6, arg7)
int arg1;
int arg2;
Point arg3;
int arg4;
int arg5;
Point arg6;
Point arg7;
{
	sendCommand(CCTRACKELARC);
	sendInt(arg1);
	sendInt(arg2);
	sendPoint(arg3);
	sendInt(arg4);
	sendInt(arg5);
	sendPoint(arg6);
	sendPoint(arg7);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackeldisc(arg1, arg2, arg3, arg4, arg5)
int arg1;
int arg2;
Point arg3;
int arg4;
int arg5;
{
	sendCommand(CCTRACKELDISC);
	sendInt(arg1);
	sendInt(arg2);
	sendPoint(arg3);
	sendInt(arg4);
	sendInt(arg5);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackellipse(arg1, arg2, arg3, arg4, arg5)
int arg1;
int arg2;
Point arg3;
int arg4;
int arg5;
{
	sendCommand(CCTRACKELLIPSE);
	sendInt(arg1);
	sendInt(arg2);
	sendPoint(arg3);
	sendInt(arg4);
	sendInt(arg5);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackline(arg1, arg2, arg3, arg4)
int arg1;
int arg2;
Point arg3;
Point arg4;
{
	sendCommand(CCTRACKLINE);
	sendInt(arg1);
	sendInt(arg2);
	sendPoint(arg3);
	sendPoint(arg4);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackrect(arg1, arg2, arg3)
int arg1;
int arg2;
Rectangle arg3;
{
	sendCommand(CCTRACKRECT);
	sendInt(arg1);
	sendInt(arg2);
	sendRectangle(arg3);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackrectf(arg1, arg2, arg3)
int arg1;
int arg2;
Rectangle arg3;
{
	sendCommand(CCTRACKRECTF);
	sendInt(arg1);
	sendInt(arg2);
	sendRectangle(arg3);
	flush();
	clearMouseQ();
	return recPoint();
}

Point trackstring(arg1, arg2, arg3, arg4, arg5)
int arg1;
int arg2;
Point arg3;
char *arg4;
Font *arg5;
{
	sendCommand(CCTRACKSTRING);
	sendInt(arg1);
	sendInt(arg2);
	sendPoint(arg3);
	sendToChar(arg4);
	sendToFont(arg5);
	flush();
	clearMouseQ();
	return recPoint();
}

