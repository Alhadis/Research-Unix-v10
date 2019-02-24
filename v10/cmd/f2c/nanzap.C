 static void
nanzap()
{
	long xb[10000];
	register long *x, *xe;
	x = xb, xe = x + 10000;
	if ((long)x & 4)
		x++;
	for(; x < xe; x += 2) {
		x[0] = 0x7fff8000;
		x[1] = 0x7fc00000;
		}
	}
