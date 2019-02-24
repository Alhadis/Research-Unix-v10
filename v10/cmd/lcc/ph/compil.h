/* As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* CONFIGURATION -- ADAPT THE FOLLOWING PAGE OF DEFINITIONS TO LOCAL VALUES APPROPRIATE TO COMPILER  */

#ifndef COMPILER_H
#define COMPILER_H
#define ANSI 1	/* conforms to ANSI/IOS C standard */
#define V7	 0	
/* The following named-constants can be configured independently */
#define UNIQ_MEMB_NAMES 	1	/* Does each struct-or-union have unique member name table */
#define VOID_OK 			1	/* Does compiler recognize "void" */
#define LONG_IDENTIFIERS 	1	/* Is name significance at least 31 */
#define STRUCT_COND_OK 		1	/* Can conditional (?:) operator take struct-or-union operands */
#define TAG_AND_MEMB_SPACES 1	/* Are name spaces of tags and members separate */
#define ALL_STRING_FNS 		1	/* Are all string fns (see d11.c pg 1) provided */
#define ALL_CTYPE_FNS 		1	/* Are all ctype fns (esp. isgraph, tolower, toupper) provided */
#define HAS_PROTOTYPES		1	/* Does compiler support "prototype" declarations -- int f(int); */
#define NEW_STYLE_FN_DEF 	1	/* Does compiler support "prototype" fn definitions -- int f(int i) { ... } */
#define MACRO_NOV_86 		1	/* Full Nov 86 draft -- rescan cat'ed names, full re-scan */
#define LIB_NOV_86			1	/* Full Nov 86 library */
#define WIDE_CHARS			1	/* has L'x', L"x", and wc*mb* functions */
#define PP_CAT_AND_STR		1	/* has preprocessor catenate and stringize */
#define TRIGRAPHS			1	/* supports trigraphs */
#define ANSI8612			1	/* embodies Dec 86 decisions (see Suite User Manual) */
#define ANSI8703			1	/* embodies Mar 87 decisions (see Suite User Manual) */
#define ANSI8706			1	/* embodies Jun 87 decisions (see Suite User Manual) */
#define ANSI8709			1	/* embodies Sep 87 decisions (see decision.88 */
#define ANSI8712			1	/* embodies Dec 87 decisions (see decision.88 */
#define ANSI8804			1	/* embodies Apr 88 decisions (see decision.88 */
#define ANSI8809			1	/* embodies Sep 88 decisions (see decision.88 */
#if !LIB_NOV_86
#include "stubsh.h"
#endif	/* LIB_NOV_86 */
#ifndef LIB_TEST
#include <limits.h>
#include <float.h>
#include <stddef.h>
#if ANSI8703	/* to get errno */
#include <errno.h>
#endif	/* ANSI8703 */
#define LIB_TEST 0
#endif	/* !LIB_TEST */

#define UCHAR	unsigned char		/* For pre-ANSI compiler, these */
#define USHORT	unsigned short		/* can be simulated or faked */
#define ULONG	unsigned long		/* as needed */
#ifndef Dgeneric_ptr
#define Dgeneric_ptr
typedef void *generic_ptr;
#endif	/* Dgeneric_ptr */
#endif	/* COMPILER_H */
