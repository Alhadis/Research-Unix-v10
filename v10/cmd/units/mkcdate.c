/*% cyntax % && cc -go # %
 */
#include <sys/types.h>
main(){
	time_t time(), t=time((time_t *)0);
	char *ctime();
	printf("time_t cdate=%d; /* %.24s */\n",
		t, ctime(&t));
	exit(0);
}
