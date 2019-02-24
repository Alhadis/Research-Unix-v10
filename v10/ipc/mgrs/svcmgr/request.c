#include "mgr.h"

/*
 *  create a new request.  the request is staticly allocated since
 *  only one exists per process.
 */
Request *
newrequest(ip, sp)
	ipcinfo *ip;
	Service *sp;
{
	static Request r;

	r.i = ip;
	r.s = sp;
	r.line = r.args = r.term = NULL;
	r.errfd = -1;
	return &r;
}

freerequest(rp)
	Request *rp;
{
	if(rp->line)
		free(rp->line);
	if(rp->args)
		free(rp->args);
}

/*
 *  execute a request
 */
doreq(rp)
	Request *rp;
{
	Action *ap;

	for(ap=rp->s->ap; ap; ap=ap->next) {
		if((*ap->func)(rp, ap)<0){
			logstatus("in", rp->i);
			return;
		}
	}
}
