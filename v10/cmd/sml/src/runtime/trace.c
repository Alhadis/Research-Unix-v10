/* Copyright 1989 by AT&T Bell Laboratories */
#include "tags.h"
#include "descriptor.h"
trace(to_low, target_lo, target_hi) int *to_low, *target_lo, *target_hi;
{int *x=to_low;
 chatting("tracing %8x\n", target_lo);
  while(x<target_lo)
   {int *p= x+1, *x0= x;
    if (contains_no_ptrs(*x)) 
	x+=((get_len(x)+7)>>2);
    else 
      {x += get_len(x)+1;
       do if ((((*p)&1)==0) && *p >= (int)target_lo && *p < (int)target_hi)
            {trace(to_low,x0,x);
	     printrec(x0);
	     return;
	    }
         while (++p < x);
      }
   }
}

printrec(x) int *x;
{int len = get_len(x), i;
    chatting("\nAddress %8x:    tag=%8x\n",x+1,*x);
    for(i=0;i<len;i++)
     chatting("[%2x] %8x\n",i,x[i+1]);
}
