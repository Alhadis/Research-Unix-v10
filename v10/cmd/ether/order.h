/*
 *             O R D E R . H
 *
 * Used to re-arrange shorts and longs to or from a specific byte ordering.
 * The hfirst macros are for high-order byte first; the lfirst macros are
 * for low-order byte first.
 *
 * IMPORTANT: The definitions of order_hfirst and order_lfirst may have to be
 *   exchanged, depending on the native byte ordering of the machine.
 *
 */

#define hfirst_short(x)  ((short)order_hfirst((x), sizeof(short)))
#define lfirst_short(x)  ((short)order_lfirst((x), sizeof(short)))

#define hfirst_long(x)   ((long)order_hfirst((x), sizeof(long)))
#define lfirst_long(x)   ((long)order_lfirst((x), sizeof(long)))

#define order_hfirst(x,size)  switch_order((long)(x), size)
#define order_lfirst(x,size)  (long)(x)

long switch_order();
