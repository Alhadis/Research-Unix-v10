#include <jerq.h>
struct Proc proctab[2];
main(){
	int x=(char *)&proctab[1].state-(char *)&proctab[0];
}
