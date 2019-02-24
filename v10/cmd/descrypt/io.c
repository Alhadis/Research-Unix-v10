/*
 *	Machine Independent Input/Output Routines
 *	D.P.Mitchell  83/06/08.
 */

#include <stdio.h>
#include "crypt.h"
#define MAGIC 0xe48d

extern Block random;
extern int permutation[];
extern int pflag;

/*
 *	Traffic-encryption layer (plaintext source)
 */
int
p_source(output)
register Block *output;
{
	static int state = FIRST8;
	static long nblocks;
	static long rand;
	register c;
	register n;
	int all_junk;

	output->left  = 0;
	output->right = 0;
	all_junk = 1;
	for (n = 0; n < 64; n += 8) {
		switch (state) {
		case FIRST8:
			make_sum(&random);
			*output = random;
			state = GETCHAR;
			nblocks = 1;
			return 1;
		case GETCHAR:
			all_junk = 0;
			c = getchar();
			if (c == EOF) {
				state = ENDTEXT;
				c = ETX;
				break;
			}
			if (c == ETX) {
				state = REPEATETX;
				break;
			}
			break;
		case REPEATETX:
			all_junk = 0;
			state = GETCHAR;
			c = ETX;
			break;
		case ENDTEXT:
			state = PADJUNK;
			rand = NEXT(random.left);
			for (;;) {
				c = NEXT(rand);
				if (((c >> 24) & 0377) != ETX)
					break;
				rand = NEXT(rand);
			}
		case PADJUNK:
			if (all_junk && nblocks % SUPERSIZE == 0)
				return 0;
			rand = NEXT(rand);
			c = (rand >> 24) & 0377;
			break;
		}
		if (n > 31)
			output->right |= c << (n & 31);
		else
			output->left  |= c << (n & 31);
	}
	nblocks++;
	return 1;
}

/*
 *	traffic-decryption layer (plaintext sink)
 */
p_sink(input)
register Block *input;
{
	static int state = FIRST8;
	register c;
	register n;

	for (n = 0; n < 64; n += 8) {
		if (n > 31)
			c = (input->right >> (n & 31)) & 0377;
		else
			c = (input->left  >> (n & 31)) & 0377;
		switch (state) {
		case FIRST8:
			check_sum(input);
			state = PUTCHAR;
			return;
		case PUTCHAR:
			if (c == ETX) {
				state = FOUNDETX;
				break;
			}
			putchar(c);
			break;
		case FOUNDETX:
			if (c == ETX) {
				state = PUTCHAR;
				putchar(ETX);
				break;
			}
			state = ENDTEXT;
			break;
		case ENDTEXT:
			break;
		}
	}
}

/*
 *	permutation-decryption layer (ciphertext source)
 */
int
c_source(output)
Block *output;
{
	register long n;
	register int *ip;
	static int c_buffer[128];
	static index = 128;

	if (index > 127) {
		if (pflag) {
			if (!in_alpha(c_buffer))
				return 0;
		} else {
			for (n = 0; n < 128; n++)
				if ((c_buffer[permutation[n]] = getchar()) == EOF)
					return 0;
		}
		index = 0;
	}
	ip = &c_buffer[index];
	n  = *ip++;
	n |= *ip++ << 8;
	n |= *ip++ << 16;
	n |= *ip++ << 24;
	output->left = n;
	n  = *ip++;
	n |= *ip++ << 8;
	n |= *ip++ << 16;
	n |= *ip << 24;
	output->right = n;
	index += 8;
	return 1;
}

/*
 *	permutation-encryption layer (ciphertext sink)
 */
c_sink(input)
Block *input;
{
	register long n;
	register int *ip;
	static int c_buffer[128];
	static int index = 0;

	ip = &c_buffer[index];
	n = input->left;
	*ip++ = n & 0xff;
	*ip++ = (n >> 8) & 0xff;
	*ip++ = (n >> 16) & 0xff;
	*ip++ = (n >> 24) & 0xff;
	n = input->right;
	*ip++ = n & 0xff;
	*ip++ = (n >> 8) & 0xff;
	*ip++ = (n >> 16) & 0xff;
	*ip   = (n >> 24) & 0xff;
	index += 8;
	if (index > 127) {
		if (pflag)
			out_alpha(c_buffer);
		else {
			for (n = 0; n < 128; n++)
				putchar(c_buffer[permutation[n]]);
		}
		index = 0;
	}
}

make_sum(block)
Block *block;
{
	long a, b, c;
	long sum;

	a = block->right & 0xffff;
	b = (block->right >> 16) & 0xffff;
	c = block->left  & 0xffff;
	sum = (MAGIC + 13*a + 23*b + 31*c) & 0xffff;
	block->left = (sum << 16) | c;
}

check_sum(block)
Block *block;
{
	long a, b, c, d;
	long sum;

	a = block->right & 0xffff;
	b = (block->right >> 16) & 0xffff;
	c = block->left  & 0xffff;
	d = (block->left  >> 16) & 0xffff;
	sum = (MAGIC + 13*a + 23*b + 31*c) & 0xffff;
	if (sum != d) {
		fprintf(stderr, "Wrong key\n");
		exit(1);
	}
}
