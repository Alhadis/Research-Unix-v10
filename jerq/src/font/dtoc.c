/*
	convert a disc file to defont.c format

	usage: jx dtoc < fontfile > junk.c
*/

#include <jerq.h>
#include <jerqio.h>
#include <font.h>

main()
{
	register c;
	register Font *f;
	register Word *p;
	register Fontchar *in;
	int n, i;
	extern getchar();

	if((f = infont(getchar)) == (Font *)0)
	{
		printf("infont failed\n");
		exit(1);
	}
	string(f, "abcdefghijklmnop", &display, Drect.origin, F_XOR);

	printf("#define		DEFONT\n");
	printf("#include <jerq.h>\n");
	printf("#include <font.h>\n\n");
	printf("static int bits[] = {\n");
	p = f->bits->base;
	n = f->height*f->bits->width;
	for (i = 0; i < n; i++) {
		printf("	0x%x,",*p++);
		if ((i&7) == 7)
			printf("\n");
	}
	printf("};\n\n");
	printf("static Bitmap strike = {\n	bits,\n");
	printf("	%d,\n",f->bits->width);
	printf("	%d, %d, %d, %d,\n", f->bits->rect.origin.x,
		f->bits->rect.origin.y, f->bits->rect.corner.x, f->bits->rect.corner.y);
	printf("	0,\n};\n\n");
	printf("struct\n");
	printf("{\n");
	printf("	short n;		/* number of chars in font */\n");
	printf("	char height;		/* height of bitmap */\n");
	printf("	char ascent;		/* top of bitmap to baseline */\n");
	printf("	long unused;		/* in case we think of more stuff */\n");
	printf("	Bitmap *bits;		/* where the characters are */\n");
	printf("	Fontchar info[%d];	/* n+1 character descriptors */\n", f->n+1);
	printf("}\n");
	printf("defont = {\n");
	printf("	%d,\n", f->n);
	printf("	%d,\n	%d,\n	%d,\n",f->height,f->ascent,0);
	printf("	&strike,\n");
	printf("	{\n");
	for (c = 0; c <= f->n; c++) {
		in = f->info+c;
		printf("\t\t{ %d,	%d,	%d,	%d,	%d },\n",
			in->x,in->top,in->bottom, (0x80&in->left)?
			in->left|0xffffff00:in->left, in->width);
	}
	printf("	}\n");
	printf("};\n");
	exit();
}
