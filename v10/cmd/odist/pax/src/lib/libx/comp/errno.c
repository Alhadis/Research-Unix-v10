/*
 * this avoids multiple definitions with some libc's
 * that define both a libx supplied routine and errno
 * in the same .o
 */

int	errno;
