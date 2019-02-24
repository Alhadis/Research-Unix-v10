long switch_order(x, size)
     long           x;
     register int   size;
{
     register char  *rp, *xp;
     long           result;

     result = 0L;
     rp = (char *)&result + size;
     xp = (char *)&x;
     while (size-- > 0)
          *--rp = *xp++;

     return result;
}
