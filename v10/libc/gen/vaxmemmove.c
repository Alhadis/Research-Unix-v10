#define CHUNK 65532	/* word boundary */

/* VAX memcpy is OK up to 65535-byte strings */

extern void *memcpy();

void *
memmove(vout, vin, n)
void *vout, *vin;
unsigned n;
{
	char *out, *in;
	register unsigned chunk;
	if(n <= CHUNK || vout < vin)
		return memcpy(vout, vin, n);
	chunk = n%CHUNK;
	out = (char*)vout + n;
	in = (char*)vin + n;
	while(memcpy(out-=chunk, in-=chunk, chunk) > vout)
		chunk = CHUNK;
	return vout;
}
