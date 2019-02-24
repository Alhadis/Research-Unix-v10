/*                    W A R N I N G !
 *
 * If you use setjmp/longjmp, you had better know what you are doing!
 */
#include "/usr/include/setjmp.h"

extern "C" {
	int setjmp(jmp_buf);
	void longjmp(jmp_buf, int);
}
