#include	<stdio.h>

static char *iberrs[] =
{
	"No command byte",
	"CAC conflict",
	"not CAC",
	"not SAC",
	"IFC abort",
	"timeout",
	"bad function code",
	"TCT timeout",
	"no listeners",
	"bcr error",
	"no PP",
	"ir timeout",
	"ir next memory",
	"next memory",
	"bcr-bar error",
	"UNIBUS map",
	"open error",
	"unused",
	"unused",
	"bad ibup function code",
	"no device",
	"device not Listener",
	"device not Talker",
};

iberr(err, str)
	char *str;
{
	char *s, buf[256];
	extern int errno;

	if(err >= 0)
		s = "No error";
	else if((err < 0) && (err > -24))
		s = iberrs[-1-err];
	else if(err == -100)
		sprintf(s = buf, "error -100: errno=%d\n", errno);
	else
		sprintf(s = buf, "no message for error %d", err);
	printf("%d %s: %s\n", err, str, s);
}
