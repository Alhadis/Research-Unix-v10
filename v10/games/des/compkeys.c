extern int shift[];
extern long keys_L[], keys_H[];
extern long PC1[], PC2[];

compkeys(key, rev)
long key[2];
int rev;
{
	register long low, high;
	register long lowkey, highkey;
	register long i, j;
	int iiiii;
	int carry1, carry2, carry3;

	low = 0;
	high = 0;
	for (i = 0, j = key[1]; i < 64; i += 2) {
		if (j < 0) {
			low |= PC1[i];
			high |= PC1[i + 1];
		}
		j += j;
	}
	for (j = key[0]; i < 128; i += 2) {
		if (j < 0) {
			low |= PC1[i];
			high |= PC1[i + 1];
		}
		j += j;
	}
	for (iiiii = 0; iiiii < 16; iiiii++) {
		for (j = 0; j < shift[iiiii]; j++) {
			carry1 = 0;
			carry2 = 0;
			carry3 = 0;
			if (low < 0)
				carry1 = 020;
			if (low & 010)
				carry2 = 0400;
			if (high < 0)
				carry3 = 1;
			low <<= 1;
			low &= ~020;
			low |= carry1;
			low |= carry3;
			high <<= 1;
			high |= carry2;
		}
		lowkey = 0;
		highkey = 0;
		for (i = 0, j = high; i < 64; i += 2) {
			if (j < 0) {
				lowkey |= PC2[i];
				highkey |= PC2[i + 1];
			}
			j += j;
		}
		for (j = low; i < 128; i += 2) {
			if (j < 0) {
				lowkey |= PC2[i];
				highkey |= PC2[i + 1];
			}
			j += j;
		}
		if (rev) {
			keys_L[15 - iiiii] = lowkey;
			keys_H[15 - iiiii] = highkey;
		} else {
			keys_L[iiiii] = lowkey;
			keys_H[iiiii] = highkey;
		}
	}
}
