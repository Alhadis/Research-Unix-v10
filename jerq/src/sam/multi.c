#include "sam.h"

struct Filelist	file;
ushort	tag;

File *
newfile()
{
	register File *f;
	inslist((List *)&file, 0, (long)(f=Fopen()));
	f->tag=tag++;
	if(downloaded)
		outTs(Hnewname, f->tag);
	/* already sorted; file name is "" */
	return f;
}
delfile(f)
	register File *f;
{
	register w=whichmenu(f);
	if(downloaded)
		outTs(Hdelname, f->tag);
	dellist((List *)&file, w);
	Fclose(f);
}
sortname(f)
	register File *f;
{
	register i, cmp;
	register w=whichmenu(f);
	register dupwarned=FALSE;
	dellist((List *)&file, w);
	if(f==cmd)
		i=0;
	else for(i=0; i<file.nused; i++){
		cmp=strcmp(f->name.s, file.ptr[i]->name.s);
		if(cmp==0 && !dupwarned){
			dupwarned=TRUE;
			warn_s(Wdupname, (char *)f->name.s);
		}else if(cmp<0 && (i>0 || cmd==0))
			break;
	}
	inslist((List *)&file, i, (long)f);
	if(downloaded)
		outTsS(Hmovname, f->tag, f->name.s);
}
whichmenu(f)
	File *f;
{
	register i;
	for(i=0; i<file.nused; i++)
		if(file.ptr[i]==f)
			return i;
	return -1;
}
state(f, cleandirty)
	register File *f;
	enum State cleandirty;
{
	if(f==cmd)
		return;
	if(downloaded && whichmenu(f)>=0){	/* else flist or menu */
		if(f->state==Dirty && cleandirty!=Dirty)
			outTs(Hclean, f->tag);
		else if(f->state!=Dirty && cleandirty==Dirty)
			outTs(Hdirty, f->tag);
	}
	f->state=cleandirty;
}
File *
lookfile()
{
	register i;
	for(i=0; i<file.nused; i++)
		if(strcmp(file.ptr[i]->name.s, genstr.s)==0)
			return file.ptr[i];
	return 0;
}
