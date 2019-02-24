5a6,12
>  static VOID
> nl_donewrec(Void)
> {
> 	(*f__donewrec)();
> 	PUT(' ');
> 	}
> 
33a41
> #ifdef No_Extra_Namelist_Newlines
35c43,44
< 			(*f__donewrec)();
---
> #endif
> 			nl_donewrec();
51c60
< 				(*f__donewrec)();
---
> 				nl_donewrec();
56c65
< 			(*f__donewrec)();
---
> 			nl_donewrec();
