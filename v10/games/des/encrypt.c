/*
 *	data encryption and decryption program
 */
#include <stdio.h>

typedef union {
	long words[2];
	char bytes[8];
} block;
extern long keys_L[], keys_H[];

main(argc, argv)
char *argv[];
{
	static block key;
	static block icv;
	static block in;
	register char *cp;
	register c;
	register i;
	char *cpretry;
	static char buf[512];
	extern char *getpass();

	/*
	 *	generate key by crunching user supplied string.
	 */
	if (argc > 2) {
		fprintf(stderr, "usage: %s key <stdin >stdout\n", argv[0]);
		exit(1);
	}
	if (argc == 2) {
		strncpy(buf, argv[1], 512);
		cp = argv[1];
		while (*cp)
			*cp++ = '\0';
		cp = buf;
	}
	else {
		for (cp=cpretry=(char*)NULL; ;) {
			cp = getpass("Enter Key: ");
			cp = (char*)strcpy(buf,cp);
			cpretry = getpass("Reenter Key for Verification: ");
			if (strcmp(cp,cpretry))
				fprintf(stderr,"Sorry, keys don't match.\n");
			else {
				while (*cpretry) *cpretry++ = '\0';
				break;
			}
		}
	}
	key.words[0] = 0;	/* modification April 28, 1982 */
	key.words[1] = 0;
	compkeys("canofbugs", 0);
	for (i = 0; *cp; i = ++i & 07) {
		key.bytes[i & 07] ^= *cp;
		*cp++ = '\0';
		if (i == 07)
			des(&key, &key);
	}
	des(&key, &key);
	/*
	 *	set initial chaining value
	 */
	des("numbersoup", &icv);
	cp = argv[0];
	for (i = strlen(cp); i >= 0; --i)
		if (cp[i] == '/')
			break;
	cp += i + 1;
	i = 0;
	if (strcmp(cp, "encrypt") == 0) {
		compkeys(&key, 0);
		while (i = fread(key.bytes, 1, 8, stdin)) {
			if (i != 8)
				des(&icv, &icv);
			icv.words[0] ^= key.words[0];
			icv.words[1] ^= key.words[1];
			if (i == 8)
				des(&icv, &icv);
			fwrite(icv.bytes, 1, i, stdout);
		}
	} else if (strcmp(cp, "decrypt") == 0) {
		compkeys(&key, 1);
		while (i = fread(in.bytes, 1, 8, stdin)) {
			if (i == 8) {
				des(&in, &key);
				icv.words[0] ^= key.words[0];
				icv.words[1] ^= key.words[1];
			} else {
				keyreverse();
				des(&icv, &icv);
				icv.words[0] ^= in.words[0];
				icv.words[1] ^= in.words[1];
			}
			fwrite(icv.bytes, 1, i, stdout);
			icv.words[0] = in.words[0];
			icv.words[1] = in.words[1];
		}
	} else {
		fprintf(stderr, "zeroth argument must be 'encrypt' or 'decrypt'\n");
		exit(1);
	}
}
keyreverse()
{
	register i;
	register j;

	for (i = 0; i < 8; i++) {
		j = keys_L[i];
		keys_L[i] = keys_L[15-i];
		keys_L[15-i] = j;
		j = keys_H[i];
		keys_H[i] = keys_H[15-i];
		keys_H[15-i] = j;
	}
}
