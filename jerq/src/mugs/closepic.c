/*% vcc -gc %
 */
#include "fb.h"
closepic(f)
PICFILE *f;
{
	register i;
	if(f->fd!=NULL)
		fclose(f->fd);
	if(f->file!=NULL)
		free(f->file);
	if(f->argv!=NULL){
		for(i=0;f->argv[i];i++)
			free(f->argv[i]);
		free(f->argv);
	}
	if(f->cmap!=NULL)
		free(f->cmap);
	free(f);
}
