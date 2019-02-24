#include	<jerq.h>
#include	<font.h>

main()
{
	char buf[256];

	request(KBD|MOUSE);
	rectf(&display, Drect, F_OR);
	strcpy(buf, "prompt: ");
	rectf(&display, Drect, F_XOR);
	kbdstr(buf);
	string(&defont, buf, &display, add(Drect.origin, Pt(5, 5)), F_XOR);
	sleep(300);
	strcpy(buf, "filename");
	kbordstr(buf, buf, 'b', 'a');
	string(&defont, buf, &display, add(Drect.origin, Pt(5, 25)), F_XOR);
	sleep(300);
}
