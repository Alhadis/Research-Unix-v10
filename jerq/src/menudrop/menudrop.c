#include <jerq.h>

Texture menu3 = {
	0xFFC0, 0x8040, 0x8040, 0x8040, 0xFFC0, 0xFFC0, 0xFE00, 0xFEF0,
	0x80E0, 0x80F0, 0x80B8, 0xFE1C, 0x800E, 0x8047, 0x8042, 0xFFC0,
};

Texture menu23 = {
	0x1FF8, 0x1008, 0x1008, 0x1008, 0x1FF8, 0x1FF8, 0x1FF8, 0x1C00,
	0x119E, 0x13DC, 0x119E, 0x1D97, 0x1182, 0x1008, 0x1008, 0x1FF8,
};

Texture deadmouse = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000C, 0x0082, 0x0441,
	0xFFE1, 0x5FF1, 0x3FFE, 0x17F0, 0x03E0, 0x0000, 0x0000, 0x0000,
};

char *top_menu[]={
	"drop menu",
	"mux menus",
	"cleanup",
	"exit",
	NULL
};
Menu topmenu={ top_menu };

char *demo_menu[]={
	"lift menu",
	"no selection",
	"select 0", "select 1", "select 2", "select 3",
	"select 4", "select 5", "select 6", "select 7",
	"select 8", "select 9", "select 10", "select 11",
	"select 12", "select 13", "select 14", "select 15",
	"clear cursor",
	"upper left cursor",
	"lower left cursor",
	"upper right cursor",
	"lower right cursor",
	"clear border",
	"set border",
	NULL
};
Menu demomenu={ demo_menu };

Menu *menulist[] = { &topmenu, &demomenu };

extern int (*muxhit)();

main()
{
	int choice, dropflag = 0;

	request(MOUSE);
	menudemo(0);
	cursswitch(&menu3);
	P->state |= RESHAPED;
	for (;;sleep(6)) {
		if (P->state & (MOVED|RESHAPED)) {
			P->state &= ~(MOVED|RESHAPED);
			ident(&display, Drect, "menudrop");
		}
		if (!button3() || !ptinrect(mouse.xy, Drect))
			continue;
		if (dropflag && ldead())
			menudemo(dropflag = 0);
		cursswitch(NULL);
		choice = (*muxhit)(menulist[dropflag], 3);
		if (dropflag && choice >= 0) {
			menudemo(0);
			if (choice == 0)
				menulift();
			else if ((choice -= 2) <= 15)
				menuselect(choice);
			else if ((choice -= 17) <= 3)
				docursor(choice);
			else
				doborder(choice - 4);
		} else switch (choice) {
		case 0:
			menudemo(dropflag = 1); break;
		case 1:
			muxmenus(), dropflag = 1; break;
		case 2:
			cleanup(); break;
		case 3:
			exit();
		}
		cursswitch(&menu3);
	}
}

char *mux_2[]={
	"cut",
	"paste",
	"snarf",
	"send",
	NULL,
	NULL
};
Menu mux2={ mux_2 };

char *mux_3[]={
	"New",
	"Reshape",
	"Move",
	"Top",
	"Bottom",
	"Current",
	"Memory",
	"Delete",
	NULL
};
Menu mux3={ mux_3 };

muxmenus()
{
	static scroll;
	mux_2[4] = scroll ? "noscroll" : "scroll";
	cursswitch(&menu23);
	for (;; nap(2)) {
		if (!button23())
			continue;
		if (button2()) {
			if (demohit(&mux2, 2) == 4)
				scroll = 1 - scroll;
		} else if (button3())
			demohit(&mux3, 3);
		break;
	}
}

#define OUTER	1
#define INNER	2
#define Border(b, r, f)	border(b, inset(r, OUTER), INNER, f)

doborder(flag)
{
	register struct Proc *p;
	cursswitch(&deadmouse);
	while ((p = getproc()) == 0)
		sleep(6);
	if (p != P)
		Border(p->layer, p->layer->rect, flag ? F_STORE : F_CLR);
}
