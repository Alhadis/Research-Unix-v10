#include	<getflags.h>
#include	<ctype.h>
#include	<fio.h>
#include	<libc.h>
#include	"crossbar.h"
#include	"switch.h"

static Crossbar *c1, *c2, *r1;
static Switch *rgb, *comp;
int verbose;
void init(), alarminit();

main(int argc, char **argv)
{
	Switch *sw;
	int i;
	char opt;

	if((argc = getflags(argc, argv, "dcrvpi", 0)) < 0)
		usage("[ iopair ... ]");
	verbose = flag['v'] != 0;
	alarminit();
	init();
	if(flag['c'])
		sw = comp, opt = 'c';
	else if(flag['r'])
		sw = rgb, opt = 'r';
	else {
		fprint(2, "%s: must specify one of c(omposite) or r(gb)\n", argv[0]);
		usage("[ iopair ... ]");
	}
	if(flag['d']){
		sw->describe();
		exit(0);
	}
	if(flag['i'])
		sw->reinit();
	for(i = 1; i < argc; i++){
		if(!isalnum(argv[i][0]) || !isalnum(argv[i][1]) || argv[i][2]){
			fprint(2, "vsw: bad iopair '%s'\n", argv[i]);
			exit(1);
		}
		sw->connect(argv[i][0], argv[i][1]);
	}
	if(flag['p'])
		sw->pr(opt);
	exit(0);
}

void
init()
{
	c1 = new System10("vswcomp", 10, 1);
	c2 = new Dynasty("vswrgb2", 20, 1);
	r1 = new System10("vswrgb", 10, 3);
	rgb = new Switch("rgb", 3);
	rgb->addi('0', "pixel", "pixel machine", r1, 0);
	rgb->addi('1', "bad", "bad blue 11 12 13 14", r1, 1);
	rgb->addi('2', "iti", "iti frame grabber", c1, 3);
	rgb->addi('3', "pipe /dev/om0", "pipe /dev/om0", r1, 2);
	rgb->addi('4', "pipe /dev/om1", "pipe /dev/om1", r1, 3);
	rgb->addi('5', "pipe /dev/om2", "pipe /dev/om2", r1, 4);
	rgb->addi('6', "pipe /dev/om3", "pipe /dev/om3", r1, 5);
	rgb->addi('7', "arend /dev/om2", "arend /dev/om2", r1, 6);
	rgb->addi('8', "encke /dev/om0", "encke /dev/om0", r1, 7);
	rgb->addi('9', "spare", "spare", r1, 8);
	rgb->addi('a', "camera", "sony camera rgb output", c1, 0);
	r1->link(9, c1, 6);
	rgb->addo('0', "ntsc, MitsRGB", "rgb->ntsc, 35in Mitsubishi RGB", r1, 0);
	rgb->addo('1', "matrix", "matrix 6000, monitor", r1, 1);
	rgb->addo('2', "iti", "iti frame buffer", c1, 3);
	rgb->addo('3', "ws1", "work station 1", r1, 2);
	rgb->addo('4', "ws2", "work station 2", r1, 8);
	rgb->addo('5', "ws3", "work station 3", r1, 4);
	rgb->addo('6', "ws4", "work station 4", r1, 5);
	rgb->addo('7', "ws5", "work station 5", r1, 6);
	rgb->addo('8', "ws6", "work station 6", r1, 7);
	rgb->addo('9', "unused/bad", "work station 7", r1, 3);
	rgb->addo('a', "barco/proj", "barco mon/projector tv", c1, 0);
	c1->link(6, r1, 9);
	comp = new Switch("composite", 1);
	comp->addi('0', "vhs1", "vhs1(source) output", c2, 0);
	comp->addi('1', "vhs2", "vhs2(editor) output", c2, 1);
	comp->addi('2', "ntsc", "rgb->ntsc output", c2, 2);
	comp->addi('3', "dig.audio", "tom's digital audio", c2, 3);
	comp->addi('4', "pal", "PAL/SECAM player", c2, 4);
	comp->addi('5', "2500.sig", "2500 signal output", c2, 5);
	comp->addi('6', "2500.mon", "2500 mon", c2, 6);
	comp->addi('7', "umatic", "umatic output", c2, 7);
	comp->addi('8', "cbars", "signal generator", c2, 8);
	comp->addi('9', "kodak", "kodak camera disc", c2, 9);
	comp->addi('a', "podium", "graphics lab podium output", c2, 10);
	comp->addi('b', "camera.comp", "sony composite output", c2, 11);
	comp->addi('c', "630", "630 video adapter", c2, 12);

	comp->addo('0', "vhs1", "vhs1(source) input", c2, 0);
	comp->addo('1', "vhs2", "vhs2(editor) input", c2, 1);
	comp->addo('2', "ntsc", "ntsc->rgb input", c2, 2);
	comp->addo('3', "dig.audio", "tom's digital audio", c2, 3);
	comp->addo('4', "pal", "PAL/SECAM input", c2, 4);
	comp->addo('5', "2500", "2500 signal input", c2, 5);
	comp->addo('6', "ikegami", "ikegami mon B input", c2, 6);
	comp->addo('7', "sm1", "left 7\" rack mon B input", c2, 7);
	comp->addo('8', "sm2", "right 7\" rack mon B input", c2, 8);
	comp->addo('9', "mits1", "39\" mon comp 1 input", c2, 9);
	comp->addo('a', "unsused", "unused", c2, 10);
	comp->addo('b', "unsused", "unused", c2, 11);
	comp->addo('c', "barco1", "barco mon comp 1 input", c2, 12);
	comp->addo('d', "barco2", "barco mon comp 2 input", c2, 13);
	comp->addo('e', "dunn", "dunn camera input", c2, 14);
	comp->addo('f', "projector", "projector tv", c2, 15);
}

#include	<signal.h>
#include	<sysent.h>

void
alarming(int)
{
	signal(SIGALRM, alarming);
}

void
alarminit()
{
	signal(SIGALRM, alarming);
}
