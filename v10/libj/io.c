#include <stdio.h>
#include <sgtty.h>
#include "/usr/jerq/include/jioctl.h"
#include "jerq.h"
#include "io.h"
#include "balloc.h"
#include "commands.h"

extern int lookUpFont();
extern Font *fntab[];
extern Font *loadFont();

Rectangle Jrect, Drect;
int Jin, Jout;

#define mouseQsize 20
Mouse mouseQ[mouseQsize];
int Qnum = 0;
int lookMouse = 0; nextMouse = 0;

struct sgttyb ttyisave, ttyosave, ttymodes;

int jinit()
{	
	if (system("32ld /usr/jerq/lib/term.m")) {
		fprintf(stderr,"main: can't boot terminal program\n");
		return(0);
	}
	Jin = open("/dev/tty",0);
	Jout = open("/dev/tty",1);
	if (Jin<0 | Jout <0) {
		fprintf(stderr,"couldn't open /dev/tty\n");
		return(0);
	}
	ioctl(Jin, TIOCGETP, &ttymodes);
	ttyisave = ttymodes;
	ttymodes.sg_flags |= RAW;
	ttymodes.sg_flags &= ~ECHO;
	ioctl(Jin, TIOCSETP, &ttymodes);

	ioctl(Jout, TIOCGETP, &ttymodes);
	ttyosave = ttymodes;
	ttymodes.sg_flags |= RAW;
	ttymodes.sg_flags &= ~ECHO;
	ioctl(Jout, TIOCSETP, &ttymodes);

	Jrect.origin.x = Jrect.origin.y = 0;
	Jrect.corner.x = XMAX; Jrect.corner.y = YMAX;
	Drect.origin.x = recInt();
	Drect.origin.y = recInt();
	Drect.corner.x = recInt();
	Drect.corner.y = recInt();
	BMT[0].rect = Drect;

	fntab[0] = loadFont("/usr/jerq/font/defont");
	return(1);
}

#define BUFsize 32
char obuf[BUFsize], ibuf[BUFsize];
int pobuf = 0, pibuf = 0, pinum = 0;

void flush()
{
	if (pobuf>0) write(Jout,obuf,pobuf);
	pobuf = 0;
}
	
put(c) char c;
{
	if (pobuf == BUFsize) flush();
	obuf[pobuf++] = c;
}

char get()
{
	if (pibuf==pinum) {
		pinum = read(Jin,ibuf,BUFsize);
		pibuf = 0;
	}
	return ibuf[pibuf++];
}
	
void sendChar(c) char c;
{
	put(c);
}

void sendInt(n) int n;
{
	if (n<0) {
		put('-');
		n = -n;
	}
	for ( ; n != 0; n >>= 4) put((n&15)+'A');
	put(' ');
}


void sendPoint(p) Point p;
{
	sendInt(p.x);
	sendInt(p.y);
}

void sendRectangle(r) Rectangle r;
{
	sendInt(r.origin.x);
	sendInt(r.origin.y);
	sendInt(r.corner.x);
	sendInt(r.corner.y);
}

void sendToBitmap(b) Bitmap *b;
{
	if (b<BMT || b>BMT+BMTsize || b->jname<0) 
		fprintf(stderr,"inappropriate bitmap\n");
	else sendInt(b->jname);
}

void sendToFont(f) Font *f;
{
	sendInt(lookUpFont(f));
}


#define TXTsize 20
Texture *txp[TXTsize];
Texture txtab[TXTsize];

void sendTexture(i,t) int i; Texture *t;
{	int j;
	sendInt(i);
	txp[i] = t;
	txtab[i] = *t;
	for (j=0; j<16; j++) sendInt(t->bits[j]);
}
#define TNULL (Texture *)NULL
		
void sendToTexture(t) Texture *t;
{	int i, j;
	if (t==TNULL) {
		sendInt(-1);
		sendInt(-1);
	}
	else {
		for (i=0; i<TXTsize && txp[i]!=TNULL && txp[i]!=t; i++) ;
		if (i>=TXTsize) {
			fprintf(stderr,"out of space in TXTAB\n");
			exit();
		}
		else if (txp[i]==TNULL) sendTexture(i,t);
		else {
			for (j=0; j<16 && t->bits[j]==txp[i]->bits[j]; j++) ;
			if (j==16) {
				sendInt(-1);
				sendInt(i);
			}
			else sendTexture(i,t);
		}
	}
}

void sendToChar(str) char *str;
{	int i = 0;
	do { put(str[i]); } while (str[i++] != 0); 
}

void sendToToChar(cpp) char **cpp;
{
	sendToChar(*cpp);
}

void sendUnsignedInt(n) unsigned int n;
{
	for ( ; n != 0; n >>= 4) put((n&15)+'A');
	put(' ');
}

Point recPoint()
{	Point p;
	p.x = recInt();
	p.y = recInt();
	return(p);
}

Rectangle recRectangle()
{	Rectangle r;
	r.origin.x = recInt();
	r.origin.y = recInt();
	r.corner.x = recInt();
	r.corner.y = recInt();
	return(r);
}

int recChar()
{	int c;
	return get();
}

int recInt()
{	char c; 
	register n, i;
	int sign = 1;
	
	if ((c = get()) == '-') {
		sign = -1;
		c = get();
	}
	for (n=0, i=0; (c >= 'A' && c <= 'P'); c = get(), i+=4) 
		n += (c - 'A')<<i;
	return (n * sign);
}

Texture *recToTexture()
{	int t;
	t = recInt();
	return(&txtab[t]);
}

void readMouse()
{	int i, j;
	sendCommand(CCGETMOUSE);
	flush();
	Qnum = recInt();
	for (j=0; j<Qnum; j++) {
		for (mouseQ[j].xy.x=0, i=0; i<4*3; i+=4) 
			mouseQ[j].xy.x |= (get() << i);
		for (mouseQ[j].xy.y=0, i=0; i<4*3; i+=4) 
			mouseQ[j].xy.y |= (get() << i);
		mouseQ[j].buttons = get();
	}
	get(); /* dump the carriage return */

}

clearMouseQ()
{
	Qnum = 0;
}

Point mousexy()
{
	if (nextMouse >= Qnum) {
		readMouse();
		lookMouse = nextMouse = 0;
	}
	lookMouse  = 0;
	return(mouseQ[nextMouse++].xy);
}

int mousebuttons()
{	
	if (lookMouse) nextMouse++;
	if (nextMouse >= Qnum) {
		readMouse();
		lookMouse = nextMouse = 0;
	}
	lookMouse = 1;
	return(mouseQ[nextMouse].buttons);
}

int kbdbuf[50];
int kbdp = 0;
int kbdn = 0;

int kbdchar()
{	int c;
	if (kbdp >= kbdn) {
		sendCommand(CCKBDCHAR);
		flush();
		kbdn = kbdp = 0;
		while ((c = recInt()) != -1) kbdbuf[kbdn++] = c;
	}
	return((kbdn == 0)? -1 : kbdbuf[kbdp++]);	
}
		
void jexit()
{	struct sgttyb buf;
	sendCommand(CCEXIT);
	flush();
	ioctl(Jin, TIOCSETP, &ttyisave);
	ttyosave.sg_flags &= ~RAW;
	ttyosave.sg_flags |= ECHO;
	ioctl(Jout, TIOCSETP, &ttyosave);
	ioctl(Jout, JTERM, &ttyosave);
	close(Jin);
	close(Jout);
	exit();
}
	
