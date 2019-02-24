13c13
< 		f__recpos += *number * len;
---
> 		f__recpos += (int)(*number * len);
15c15
< 			err(f__elist->ciend, 110, "do_us");
---
> 			err(f__elist->cierr, 110, "do_us");
33c33
< 	f__recpos += *number * len;
---
> 	f__recpos += (int)(*number * len);
