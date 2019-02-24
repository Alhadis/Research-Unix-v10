67c67
< 		l_eof = f__curunit->uend = 1;
---
> 		f__curunit->uend = l_eof = 1;
127c127
< 			err(f__elist->cierr,112,"bad repetition count")
---
> 			errfl(f__elist->cierr,112,"bad repetition count");
160c160
< 			err(f__elist->cierr,112,"exponent field")
---
> 			errfl(f__elist->cierr,112,"exponent field");
203c203
< 				err(f__elist->cierr,112,"invalid number")
---
> 				errfl(f__elist->cierr,112,"invalid number");
239c239
< 				err(f__elist->cierr,112,"complex format")
---
> 				errfl(f__elist->cierr,112,"complex format");
245c245
< 				err(f__elist->cierr,112,"no star")
---
> 				errfl(f__elist->cierr,112,"no star");
263c263
< 		err(f__elist->cierr,112,"no real part");
---
> 		errfl(f__elist->cierr,112,"no real part");
268c268
< 		err(f__elist->cierr,112,"no comma");
---
> 		errfl(f__elist->cierr,112,"no comma");
275c275
< 		err(f__elist->cierr,112,"no imaginary part");
---
> 		errfl(f__elist->cierr,112,"no imaginary part");
277c277
< 	if(ch!=')') err(f__elist->cierr,112,"no )");
---
> 	if(ch!=')') errfl(f__elist->cierr,112,"no )");
294c294
< 				err(f__elist->cierr,112,"no star")
---
> 				errfl(f__elist->cierr,112,"no star");
315c315
< 		else	err(f__elist->cierr,112,"logical");
---
> 		else	errfl(f__elist->cierr,112,"logical");
331,332c331,333
< 	p=f__lchar=malloc((unsigned int)size);
< 	if(f__lchar==NULL) err(f__elist->cierr,113,"no space");
---
> 	p=f__lchar = (char *)malloc((unsigned int)size);
> 	if(f__lchar == NULL)
> 		errfl(f__elist->cierr,113,"no space");
368c369
< 				f__lchar = realloc(f__lchar,
---
> 				f__lchar = (char *)realloc(f__lchar,
400c401
< 				f__lchar = realloc(f__lchar,
---
> 				f__lchar = (char *)realloc(f__lchar,
413c414,415
< 			f__lchar= realloc(f__lchar, (unsigned int)(size += BUFSIZE));
---
> 			f__lchar= (char *)realloc(f__lchar,
> 					(unsigned int)(size += BUFSIZE));
528c530
< 				err(f__elist->cierr,errno,"list in")
---
> 				errfl(f__elist->cierr,errno,"list in");
536c538
< 			Ptr->flchar = f__lx;
---
> 			Ptr->flchar = (char)f__lx;
540c542
< 			Ptr->flshort=f__lx;
---
> 			Ptr->flshort = (short)f__lx;
600a603
> 	f__doend = xrd_SL;
