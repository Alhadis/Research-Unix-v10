/*
 * K. P. Vo
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * ``the best'' combined linear congruent checksum/hash/PRNG
 */

#ifndef HASHPART

#define HASH_ADD(h)	(0x9c39c33dL)

#if __sparc__ || sparc

#define HASH_A(h,n)	((((h) << 2) - (h)) << (n))
#define HASH_B(h,n)	((((h) << 4) - (h)) << (n))
#define HASH_C(h,n)	((HASH_A(h,7) + HASH_B(h,0)) << (n))
#define HASH_MPY(h)	(HASH_C(h,22)+HASH_C(h,10)+HASH_A(h,6)+HASH_A(h,3)+(h))

#else

#define HASH_MPY(h)	((h)*0x63c63cd9L)

#endif

#define HASHPART(h,c)	(h = HASH_MPY(h) + HASH_ADD(h) + (c))

#endif
