#include "jerq.h"
#include "commands.h"
#include "io.h"

void alarm(arg1)
unsigned int arg1;
{
	sendCommand(CCALARM);
	sendUnsignedInt(arg1);
}

void arc(arg1, arg2, arg3, arg4, arg5)
Bitmap *arg1;
Point arg2;
Point arg3;
Point arg4;
Code arg5;
{
	sendCommand(CCARC);
	sendToBitmap(arg1);
	sendPoint(arg2);
	sendPoint(arg3);
	sendPoint(arg4);
	sendCode(arg5);
}

void bitblt(arg1, arg2, arg3, arg4, arg5)
Bitmap *arg1;
Rectangle arg2;
Bitmap *arg3;
Point arg4;
Code arg5;
{
	sendCommand(CCBITBLT);
	sendToBitmap(arg1);
	sendRectangle(arg2);
	sendToBitmap(arg3);
	sendPoint(arg4);
	sendCode(arg5);
}

void circle(arg1, arg2, arg3, arg4)
Bitmap *arg1;
Point arg2;
int arg3;
Code arg4;
{
	sendCommand(CCCIRCLE);
	sendToBitmap(arg1);
	sendPoint(arg2);
	sendInt(arg3);
	sendCode(arg4);
}

void cursallow()
{
	sendCommand(CCCURSALLOW);
}

void cursinhibit()
{
	sendCommand(CCCURSINHIBIT);
}

void cursset(arg1)
Point arg1;
{
	sendCommand(CCCURSSET);
	sendPoint(arg1);
}

Texture * cursswitch(arg1)
Texture *arg1;
{
	sendCommand(CCCURSSWITCH);
	sendToTexture(arg1);
	flush();
	return recToTexture();
}

void disc(arg1, arg2, arg3, arg4)
Bitmap *arg1;
Point arg2;
int arg3;
Code arg4;
{
	sendCommand(CCDISC);
	sendToBitmap(arg1);
	sendPoint(arg2);
	sendInt(arg3);
	sendCode(arg4);
}

void ellipse(arg1, arg2, arg3, arg4, arg5)
Bitmap *arg1;
Point arg2;
int arg3;
int arg4;
Code arg5;
{
	sendCommand(CCELLIPSE);
	sendToBitmap(arg1);
	sendPoint(arg2);
	sendInt(arg3);
	sendInt(arg4);
	sendCode(arg5);
}

void eldisc(arg1, arg2, arg3, arg4, arg5)
Bitmap *arg1;
Point arg2;
int arg3;
int arg4;
Code arg5;
{
	sendCommand(CCELDISC);
	sendToBitmap(arg1);
	sendPoint(arg2);
	sendInt(arg3);
	sendInt(arg4);
	sendCode(arg5);
}

void elarc(arg1, arg2, arg3, arg4, arg5, arg6, arg7)
Bitmap *arg1;
Point arg2;
int arg3;
int arg4;
Point arg5;
Point arg6;
Code arg7;
{
	sendCommand(CCELARC);
	sendToBitmap(arg1);
	sendPoint(arg2);
	sendInt(arg3);
	sendInt(arg4);
	sendPoint(arg5);
	sendPoint(arg6);
	sendCode(arg7);
}

Rectangle getrect()
{
	sendCommand(CCGETRECT);
	flush();
	return recRectangle();
}

void kill(arg1)
int arg1;
{
	sendCommand(CCKILL);
	sendInt(arg1);
}

void nap(arg1)
unsigned int arg1;
{
	sendCommand(CCNAP);
	sendUnsignedInt(arg1);
}

int own(arg1)
int arg1;
{
	sendCommand(CCOWN);
	sendInt(arg1);
	flush();
	return recInt();
}

void point(arg1, arg2, arg3)
Bitmap *arg1;
Point arg2;
Code arg3;
{
	sendCommand(CCPOINT);
	sendToBitmap(arg1);
	sendPoint(arg2);
	sendCode(arg3);
}

void realtime()
{
	sendCommand(CCREALTIME);
}

void rect(arg1, arg2, arg3)
Bitmap *arg1;
Rectangle arg2;
Code arg3;
{
	sendCommand(CCRECT);
	sendToBitmap(arg1);
	sendRectangle(arg2);
	sendCode(arg3);
}

void rectf(arg1, arg2, arg3)
Bitmap *arg1;
Rectangle arg2;
Code arg3;
{
	sendCommand(CCRECTF);
	sendToBitmap(arg1);
	sendRectangle(arg2);
	sendCode(arg3);
}

void request(arg1)
int arg1;
{
	sendCommand(CCREQUEST);
	sendInt(arg1);
}

void ringbell()
{
	sendCommand(CCRINGBELL);
}

void screenswap(arg1, arg2, arg3)
Bitmap *arg1;
Rectangle arg2;
Rectangle arg3;
{
	sendCommand(CCSCREENSWAP);
	sendToBitmap(arg1);
	sendRectangle(arg2);
	sendRectangle(arg3);
}

void segment(arg1, arg2, arg3, arg4)
Bitmap *arg1;
Point arg2;
Point arg3;
Code arg4;
{
	sendCommand(CCSEGMENT);
	sendToBitmap(arg1);
	sendPoint(arg2);
	sendPoint(arg3);
	sendCode(arg4);
}

void sleep(arg1)
unsigned int arg1;
{
	sendCommand(CCSLEEP);
	sendUnsignedInt(arg1);
}

Point string(arg1, arg2, arg3, arg4, arg5)
Font *arg1;
char *arg2;
Bitmap *arg3;
Point arg4;
Code arg5;
{
	sendCommand(CCSTRING);
	sendToFont(arg1);
	sendToChar(arg2);
	sendToBitmap(arg3);
	sendPoint(arg4);
	sendCode(arg5);
	flush();
	return recPoint();
}

void texture(arg1, arg2, arg3, arg4)
Bitmap *arg1;
Rectangle arg2;
Texture *arg3;
Code arg4;
{
	sendCommand(CCTEXTURE);
	sendToBitmap(arg1);
	sendRectangle(arg2);
	sendToTexture(arg3);
	sendCode(arg4);
}

