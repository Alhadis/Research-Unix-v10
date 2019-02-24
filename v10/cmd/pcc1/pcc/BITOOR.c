 long
BITOOR(x)
 register long x;
{ return x >= 0 ? (x>>3) : -((-x)>>3); }
