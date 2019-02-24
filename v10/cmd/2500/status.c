#include	"hdr.h"

static char *msg[13][8][2] =
{
/* 0 */	0, 0,				0, 0,
	"tape mounted", "no tape",	"servo ref ok", "servo ref missing",
	"tape ok", "tape trouble",	0, "hard error",
	0, 0,				"remote", "local",
/* 1 */	0, "standby",			0, 0,
	0, "stop",			0, 0,
	0, "rew",			0, "ff",
	0, "rec",			0, "play",
/* 2 */	0, "servo lock",		0, "tso mode",
	0, "shuttle",			0, "jog",
	0, "var",			"tape fwd", "tape rev",
	0, "still",			0, "cue up",
/* 3 */	0, "auto mode",			0, 0,
	0, 0,				0, 0,
	0, "a out",			0, "a in",
	0, "out",			0, "in",
/* 4 */	0, "select e-e on",		0, "full e-e on",
	0, "search to cue",		0, "edit",
	0, "review",			0, "auto edit",
	0, "preview",			0, "preroll",
/* 5 */	0, 0,				0, "insert",
	0, "assemble",			0, "video",
	0, "sync/a4",			0, "a3",
	0, "a2",			0, "a1",
/* 6 */	0, 0,				0, "lamp still",
	0, "lamp fwd",			0, "lamp rew",
	0, "search led 8",		0, "search led 4",
	0, "search led 2",		0, "search led 1",
/* 7 */	0, "variable memory mode",	0, "var mem active",
	0, "audio split mode",		0, 0,
	0, "display hold",		0, "spot erase",
	0, 0,				0, "in-out status",
/* 8 */	0, "BUZZER ALARM",		0, "LOST LOCK",
	0, "near eot",			0, "eot",
	0, "color frame lock",		0, "SERVO ALARM",
	0, "SYSTEM ALARM",		0, "rec inhibit",
/* 9 */	0, "function abort",		0, 0,
	0, 0,				0, "video dropout",
	0, "sync dropout",		0, 0,
	0, 0,				0, 0,
/* a */	0, "rev step",			0, "fwd step",
	0, "dt step execute",		0, "dt preview",
	0, "dt rec ready",		0, "slow rec",
	0, "step rec",			0, "still rec",
/* b */	"dt rec field", "dt rec frame",	"dt rec step field", "dt rec step frame",
	"DT field", "DT frame",		"PB step field", "PB step frame",
	0, "color field detect",	0, "cf addr 4",
	0, "cf addr 2",			0, "cf addr 1",
/* c */	0, 0,				0, 0,
	0, 0,				0, 0,
	0, 0,				0, 0,
	0, "data-2",			0, "data-1",
};

status(full)
{
	char buf[64];
	register i, j;
	int hi, lo;
	char *s;

	if(cmd(0xD, "status", buf, "%c%c%c", 0x61, 0x20, BYTE(0, 0xD)))
		return;
	if(full)
		lo = 0, hi = 0xD;
	else
		lo = 0, hi = 2;
	for(j = lo; j < hi; j++){
		Fprint(1, "%x:[%2.2x] ", j, buf[j+2]&0xFF);
		for(i = 7; i >= 0; i--){
			if(s = msg[j][7-i][(buf[j+2]>>i)&1])
				Fwrite(1, s, strlen(s));
			Fputc(1, ',');
		}
		Fputc(1, '\n');
	}
	Fflush(1);
}

status1(start)
{
	char buf[64];

	if(cmd(1, "status1", buf, "%c%c%c", 0x61, 0x20, BYTE(start, 1)))
		buf[2] = 0;
	return(buf[2]);
}
