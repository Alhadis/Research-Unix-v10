416,417c416,419
< 		if((n=(*f__doed)(p,ptr,len))>0) err(f__elist->cierr,errno,"fmt");
< 		if(n<0) err(f__elist->ciend,(EOF),"fmt");
---
> 		if((n=(*f__doed)(p,ptr,len))>0)
> 			errfl(f__elist->cierr,errno,"fmt");
> 		if(n<0)
> 			err(f__elist->ciend,(EOF),"fmt");
