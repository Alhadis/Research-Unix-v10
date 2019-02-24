3a4
> #include "fmt.h" /* for f__doend */
12c13
< int i_getc(Void)
---
> static int i_getc(Void)
20c21
< 	if(f__icptr >= f__icend) err(f__svic->iciend,(EOF),"endfile");
---
> 	if(f__icptr >= f__icend) err(f__svic->iciend,(EOF),"internal read");
23a25
>  static
74a77
> 	f__doend = 0;
82c85
< s_rsni(a) icilist *a;
---
> integer s_rsni(a) icilist *a;
86c89
< s_rsni(icilist *a)
---
> integer s_rsni(icilist *a)
