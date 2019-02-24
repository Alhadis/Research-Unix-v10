179c179
< 		err(f__elist->cierr, ch, "namelist read");
---
> 		errfl(f__elist->cierr, ch, "namelist read");
261a262,287
> #ifndef No_Namelist_Questions
>  static Void
> #ifdef KR_headers
> print_ne(a) cilist *a;
> #else
> print_ne(cilist *a)
> #endif
> {
> 	flag intext = f__external;
> 	int rpsave = f__recpos;
> 	FILE *cfsave = f__cf;
> 	unit *usave = f__curunit;
> 	cilist t;
> 	t = *a;
> 	t.ciunit = 6;
> 	s_wsne(&t);
> 	fflush(f__cf);
> 	f__external = intext;
> 	f__reading = 1;
> 	f__recpos = rpsave;
> 	f__cf = cfsave;
> 	f__curunit = usave;
> 	f__elist = a;
> 	}
> #endif
> 
270c296
< 	int ch, got1, k, n, nd;
---
> 	int ch, got1, k, n, nd, quote;
288a315
>  top:
294a322,326
> #ifndef No_Namelist_Questions
> 		case '?':
> 			print_ne(a);
> 			continue;
> #endif
298c330
< 			err(a->cierr, 115, where0);
---
> 			errfl(a->cierr, 115, where0);
305c337,370
< 		err(a->cierr, 118, where0);
---
> #ifdef No_Bad_Namelist_Skip
> 		errfl(a->cierr, 118, where0);
> #else
> 	{
> 		fprintf(stderr,
> 			"Skipping namelist \"%s\": seeking namelist \"%s\".\n",
> 			buf, nl->name);
> 		fflush(stderr);
> 		for(;;) switch(GETC(ch)) {
> 			case EOF:
> 				err(a->ciend, EOF, where0);
> 			case '/':
> 			case '&':
> 			case '$':
> 				if (f__external)
> 					e_rsle();
> 				else
> 					z_rnew();
> 				goto top;
> 			case '"':
> 			case '\'':
> 				quote = ch;
>  more_quoted:
> 				while(GETC(ch) != quote)
> 					if (ch == EOF)
> 						err(a->ciend, EOF, where0);
> 				if (GETC(ch) == quote)
> 					goto more_quoted;
> 				Ungetc(ch,f__cf);
> 			default:
> 				continue;
> 			}
> 		}
> #endif
308c373
< 		err(f__elist->cierr, 113, where0);
---
> 		errfl(f__elist->cierr, 113, where0);
314c379
< 				err(a->ciend,(EOF),where0);
---
> 				err(a->ciend, EOF, where0);
330c395
< 			err(a->cierr, 119, where);
---
> 			errfl(a->cierr, 119, where);
346c411
< 					err(a->cierr, 122, where);
---
> 					errfl(a->cierr, 122, where);
349c414
< 					err(a->cierr, k, where);
---
> 					errfl(a->cierr, k, where);
351c416
< 					err(a->cierr, 115, where);
---
> 					errfl(a->cierr, 115, where);
360c425
< 			nd = dims[0];
---
> 			nd = (int)dims[0];
364c429
< 				err(a->cierr, k, where);
---
> 				errfl(a->cierr, k, where);
371c436
< 					err(a->cierr, 115, where);
---
> 					errfl(a->cierr, 115, where);
376c441
< 					err(a->cierr, k, where);
---
> 					errfl(a->cierr, k, where);
383c448
< 				err(a->cierr, 115, where);
---
> 				errfl(a->cierr, 115, where);
386c451
< 				err(a->cierr, 125, where);
---
> 				errfl(a->cierr, 125, where);
394c459
< 					err(a->cierr, k, where);
---
> 					errfl(a->cierr, k, where);
396c461
< 					err(a->cierr, 115, where);
---
> 					errfl(a->cierr, 115, where);
434c499
< 			err(a->cierr, 115, where);
---
> 			errfl(a->cierr, 115, where);
446c511,512
< 			l_read(&no1, vaddr + iva, size, type);
---
> 			if (k = l_read(&no1, vaddr + iva, size, type))
> 				return k;
459c525
< 					err(a->cierr, 125, where);
---
> 					errfl(a->cierr, 125, where);
494a561
> 	f__doend = xrd_SL;
