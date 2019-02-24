9a10,14
> #ifdef NON_UNIX_STDIO
> #ifndef unlink
> #define unlink remove
> #endif
> #else
18a24
> #endif
39c45
< 				(void) t_runc((alist *)a);
---
> 				t_runc((alist *)a);
41c47
< 				(void) fclose(b->ufd);
---
> 				fclose(b->ufd);
49,50c55,56
< 				(void) fclose(b->ufd);
< 				(void) unlink(b->ufnm); /*SYSDEP*/
---
> 				fclose(b->ufd);
> 				unlink(b->ufnm); /*SYSDEP*/
78c84
<  void
---
>  int
87a94,96
> #ifdef __cplusplus
> return 0;
> #endif
