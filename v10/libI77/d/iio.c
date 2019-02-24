15a16,18
> #ifdef __cplusplus
> 	return 0;
> #endif
68a72
> 
71a76,78
> #ifdef __cplusplus
> 	return 0;
> #endif
72a80,89
> 
>  int
> iw_rev(Void)
> {
> 	if(f__workdone)
> 		z_endp();
> 	f__hiwater = f__recpos = f__cursor = 0;
> 	return(f__workdone=0);
> 	}
> 
111c128
< 	f__dorevert = y_ierr;
---
> 	f__dorevert = iw_rev;
