#include	<ipc.h>

rx(mach, service, retry, traffic, ifd, ofd)
	char *mach, *service, *retry, *traffic;
	int *ifd, *ofd;
{
	int fd;
	char buf[256];

	if((fd = ipcopen(ipcpath(mach, "dk", service), traffic)) >= 0){
success:
		*ifd = fd;
		*ofd = dup(fd);
		return(0);
	}
	if(retry){
		if((fd = ipcopen(ipcpath(mach, "dk", retry), traffic)) < 0){
			sprint(buf, "service %s on machine %s failed: %s", retry, mach, errstr);
			perror(buf);
			return(1);
		}
		read(fd, buf, 1);
		close(fd);
	}
	if((fd = ipcopen(ipcpath(mach, "dk", service), traffic)) >= 0)
		goto success;
	sprint(buf, "service %s on machine %s failed: %s", service, mach, errstr);
	perror(buf);
	return(1);
}
