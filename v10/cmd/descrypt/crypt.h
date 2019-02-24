/*
 *	Include File for New Crypt
 *	D.P.Mitchell  83/06/28.
 */

#define NEXT(X) (0x1e1f2f2d*(X) + 0x361962e9)
#define ETX 3

/*
 *	states of traffic-encryption/decryption layer
 */
#define FIRST8 0
#define GETCHAR 1
#define REPEATETX 2
#define ENDTEXT 3
#define PADJUNK 4
#define PUTCHAR 5
#define FOUNDETX 6

/*
 *	number of DES blocks in 1024 bits
 */
#define SUPERSIZE 16

/*
 *	DES block (64 bits)
 *
 *	bit 0 is the low-order bit of block.left.
 *	bit 31 is the high-order bit (sign bit on VAX) of block.left.
 *	bit 32 is the low-order bit of block.right.
 *	bit 63 is the high-order bit of block.right.
 */
typedef struct Block {
	long left, right;
} Block;
