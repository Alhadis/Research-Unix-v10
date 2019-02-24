static unsigned response = 0;

char*
atallachal(kp)
	struct keyinfo *kp;
{
	static char buf[12];
	static int inited = 0;
	int i, j;
	char b[64], k[12];

	if(!inited)
		srand((int)time((long*)0));
	/* make a random data */
	for(i=0; i<8; buf[i++] = 0)
		;
	sprintf(buf, "%d", nrand(100000000-1000)+1000);

	/* make and save the response */
	sscanf(kp->key, "%o %o %o %o %o %o %o %o",
		&k[0], &k[1], &k[2], &k[3], &k[4], &k[5], &k[6], &k[7]);
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
			b[8*i+j] = (k[i]>>(7-j))&1;
	setkey(b);

	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
			b[8*i+j] = (buf[i]>>(7-j))&1;
	encrypt(b, 0);
	for(i=0; i<32; i++) {
		response = (response<<1) | b[i];
	}

	return buf;
}

int
atallacomp(kp, s)
	struct keyinfo *kp;
	char *s;
{
	unsigned r = 0;

	sscanf(s, "%x", &r);
	if(strlen(kp->key) > 16 && r == response)
		return 0;
	return 1;
}
