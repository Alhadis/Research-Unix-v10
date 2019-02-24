#include "task.h"

object::~object()
{
	if (o_link) task_error(E_OLINK);
	if (o_next) task_error(E_ONEXT);
} /* delete */

/*	note that a task can be on a chain in several places
*/

/* object::remember() ... */

void
object::forget(register task* p)
/* remove all occurrences of task* p from this object's task list */
{
	register olink* ll;
	register olink* l;

	if (o_link == 0) return;

	while (o_link->l_task == p) {
		ll = o_link;
		o_link = ll->l_next;
		delete ll;
		if (o_link == 0) return;
	};

	l = o_link;
	while (ll = l->l_next) {
		if (ll->l_task == p) {
			l->l_next = ll->l_next;
			delete ll;
		}
		else l = ll;
	};
}


/*
 * prepare IDLE tasks on this object for scheduling
 * also flush the remember chain
 */
void
object::alert()
{
	register olink* l;
	register olink* ll;

	for (l=o_link; ll = l; l=l->l_next, delete ll) {
		register task* p = l->l_task;
		if (p->s_state == IDLE) p->insert(0,this);
	}
	o_link = 0;
}

/*
 * virtual int object::pending() returns 1 if the object should be waited for.
 * By default say yes and assume that alert() will be called somehow.
 */
int
object::pending()
{
	return 1;
}

/*
 * object::print() is a virtual function.  In each derived class, derived::print()
 * will print the derived information, then call its immediate base::print(), with
 * second argument non-zero.  The original virtual call will have the second
 * argument 0.
 */
void
object::print(int n, int baseClass)
{
	if (!baseClass)
		printf("Unidentified object ");

	if (n&VERBOSE) {
		olink* l;
		printf("this=%d, remember_chain:\n", this);
		for (l=o_link; l; l=l->l_next) l->l_task->print(n & ~CHAIN);
	}

	if (n&CHAIN) {
		if (o_next) o_next->print(n);
	}
	printf("\n");
}

int
object::o_type()
{
	return OBJECT;
}

#define macro_start static char* error_name[] = {
#define macro(num,name,string) string,
#define macro_end(last_name) };
task_error_messages
#undef macro_start
#undef macro
#undef macro_end

static int in_error = 0;

static void
print_error(int n)
{

	register i = (n<1 || MAXERR<n) ? 0 : n;

	printf("\n\n***** task_error(%d) %s\n",n,error_name[i]);

	if (((object*)0)->this_task()) { 
		printf("thistask: ");
		thistask->print(VERBOSE|STACK);
	}
	if (sched::runchain) {
		printf("run_chain:\n");
		sched::runchain->print(CHAIN);
	}
} /* task_error */

int
object::task_error(int n)
{
	if (in_error)
		exit(in_error);
	else
		in_error = n;

	if (error_fct) {
		n = (*error_fct)(n,this);
		if (n) exit(n);
	}
	else {
		print_error(n);
		exit(n);
	}
	in_error = 0;
	return 0;
}
