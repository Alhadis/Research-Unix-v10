/* 
 * tclUtil.c --
 *
 *	This file contains utility procedures that are used by many Tcl
 *	commands.
 *
 * Copyright 1987, 1989 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/tcl/RCS/tclUtil.c,v 1.30 90/03/25 11:04:25 ouster Exp $ SPRITE (Berkeley)";
#pragma ref rcsid
#endif not lint

#define	_POSIX_SOURCE

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcl.h"
#include "tclInt.h"

/*
 *----------------------------------------------------------------------
 *
 * TclFindElement --
 *
 *	Given a pointer into a Tcl list, locate the first (or next)
 *	element in the list.
 *
 * Results:
 *	The return value is normally TCL_OK, which means that the
 *	element was successfully located.  If TCL_ERROR is returned
 *	it means that list didn't have proper list structure;
 *	interp->result contains a more detailed error message.
 *
 *	If TCL_OK is returned, then *elementPtr will be set to point
 *	to the first element of list, and *nextPtr will be set to point
 *	to the character just after any white space following the last
 *	character that's part of the element.  If this is the last argument
 *	in the list, then *nextPtr will point to the NULL character at the
 *	end of list.  If sizePtr is non-NULL, *sizePtr is filled in with
 *	the number of characters in the element.  If the element is in
 *	braces, then *elementPtr will point to the character after the
 *	opening brace and *sizePtr will not include either of the braces.
 *	If there isn't an element in the list, *sizePtr will be zero, and
 *	both *elementPtr and *termPtr will refer to the null character at
 *	the end of list.  Note:  this procedure does NOT collapse backslash
 *	sequences.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
TclFindElement(interp, list, elementPtr, nextPtr, sizePtr, bracePtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    register char *list;	/* String containing Tcl list with zero
				 * or more elements (possibly in braces). */
    char **elementPtr;		/* Fill in with location of first significant
				 * character in first element of list. */
    char **nextPtr;		/* Fill in with location of character just
				 * after all white space following end of
				 * argument (i.e. next argument or end of
				 * list). */
    int *sizePtr;		/* If non-zero, fill in with size of
				 * element. */
    int *bracePtr;		/* If non-zero fill in with non-zero/zero
				 * to indicate that arg was/wasn't
				 * in braces. */
{
    register char *p;
    int openBraces = 0;
    int size;

    /*
     * Skim off leading white space and check for an opening brace.
     */

    while (isspace(*list)) {
	list++;
    }
    if (*list == '{') {
	openBraces = 1;
	list++;
    }
    if (bracePtr != 0) {
	*bracePtr = openBraces;
    }
    p = list;

    /*
     * Find the end of the element (either a space or a close brace or
     * the end of the string).
     */

    while (1) {
	switch (*p) {

	    /*
	     * Open brace: don't treat specially unless the element is
	     * in braces.  In this case, keep a nesting count.
	     */

	    case '{':
		if (openBraces != 0) {
		    openBraces++;
		}
		break;

	    /*
	     * Close brace: if element is in braces, keep nesting
	     * count and quit when the last close brace is seen.
	     */

	    case '}':
		if (openBraces == 1) {
		    char *p2;

		    size = p - list;
		    p++;
		    if (isspace(*p) || (*p == 0)) {
			goto done;
		    }
		    for (p2 = p; (*p2 != 0) && (!isspace(*p2)) && (p2 < p+20);
			    p2++) {
			/* null body */
		    }
		    Tcl_Return(interp, (char *) NULL, TCL_STATIC);
		    sprintf(interp->result,
			    "list element in braces followed by \"%.*s\" instead of space",
			    p2-p, p);
		    return TCL_ERROR;
		} else if (openBraces != 0) {
		    openBraces--;
		}
		break;

	    /*
	     * Backslash:  skip over everything up to the end of the
	     * backslash sequence.
	     */

	    case '\\': {
		int size;

		(void) Tcl_Backslash(p, &size);
		p += size - 1;
		break;
	    }

	    /*
	     * Space: ignore if element is in braces;  otherwise
	     * terminate element.
	     */

	    case ' ':
	    case '\t':
	    case '\n':
		if (openBraces == 0) {
		    size = p - list;
		    goto done;
		}
		break;

	    /*
	     * End of list:  terminate element.
	     */

	    case 0:
		if (openBraces != 0) {
		    Tcl_Return(interp, "unmatched open brace in list",
			    TCL_STATIC);
		    return TCL_ERROR;
		}
		size = p - list;
		goto done;

	}
	p++;
    }

    done:
    while (isspace(*p)) {
	p++;
    }
    *elementPtr = list;
    *nextPtr = p;
    if (sizePtr != 0) {
	*sizePtr = size;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * TclCopyAndCollapse --
 *
 *	Copy a string and eliminate any backslashes that aren't in braces.
 *
 * Results:
 *	There is no return value.  Count chars. get copied from src
 *	to dst.  Along the way, if backslash sequences are found outside
 *	braces, the backslashes are eliminated in the copy.
 *	After scanning count chars. from source, a null character is
 *	placed at the end of dst.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void
TclCopyAndCollapse(count, src, dst)
    register char *src;		/* Copy from here... */
    register char *dst;		/* ... to here. */
{
    register char c;
    int numRead;

    for (c = *src; count > 0; dst++, src++, c = *src, count--) {
	if (c == '\\') {
	    *dst = Tcl_Backslash(src, &numRead);
	    src += numRead-1;
	    count -= numRead-1;
	} else {
	    *dst = c;
	}
    }
    *dst = 0;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_Merge --
 *
 *	Given a collection of strings, merge them together into a
 *	single string that has proper Tcl list structured (i.e.
 *	TclFindElement and TclCopyAndCollapse may be used to retrieve
 *	strings equal to the original elements, and Tcl_Eval will
 *	parse the string back into its original elements).
 *
 * Results:
 *	The return value is the address of a dynamically-allocated
 *	string containing the merged list.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_Merge(argc, argv)
    int argc;			/* How many strings to merge. */
    char **argv;		/* Array of string values. */
{
    /*
     * This procedure operates in two passes.  In the first pass it figures
     * out how many bytes will be needed to store the result (actually,
     * it overestimates slightly).  The first pass also collects information
     * about each element in the form of a flags word.  If there are only
     * a few elements, local storage gets used for the flags;  if there are
     * a lot of elements, a new array is dynamically allocated.
     *
     * In the second pass this procedure copies the arguments into the
     * result string.  The special cases to worry about are:
     *
     * 1. Argument contains embedded spaces, or starts with a brace:  must
     * add another level of braces when copying to the result.
     *
     * 2. Argument contains unbalanced braces:  backslash all of the
     * braces when copying to the result.  In this case, don't add another
     * level of braces (they would prevent the backslash from
     * being removed when the argument is extracted from the list later).
     *
     * 3. Argument contains backslashed brace/bracket:  if possible,
     * group the argument in braces:  then no special action needs to be taken
     * with the backslashes.  If the argument can't be put in braces, then
     * add another backslash in front of the sequence, so that upon
     * extraction the original sequence will be restored.
     *
     * These potential problems are the reasons why particular information
     * is gathered during pass 1.
     */
#   define WANT_PARENS			1
#   define PARENS_UNBALANCED		2
#   define PARENTHESIZED		4
#   define CANT_PARENTHESIZE		8

#   define LOCAL_SIZE 20
    int localFlags[LOCAL_SIZE];
    int *flagPtr;
    int numChars;
    char *result;
    register char *src, *dst;
    register int curFlags;
    int i;

    /*
     * Pass 1: estimate space, gather information.
     */

    if (argc <= LOCAL_SIZE) {
	flagPtr = localFlags;
    } else {
	flagPtr = (int *) malloc((unsigned) argc*sizeof(int));
    }
    numChars = 0;
    for (i = 0; i < argc; i++) {
	int braceCount, nestingLevel, nestedBS, whiteSpace, brackets, dollars;

	curFlags = braceCount = nestingLevel = nestedBS = whiteSpace = 0;
	brackets = dollars = 0;
	src = argv[i];
	if (*src == '{') {
	    curFlags |= PARENTHESIZED|WANT_PARENS;
	}
	if (*src == 0) {
	    curFlags |= WANT_PARENS;
	} else {
	    for (; ; src++) {
		switch (*src) {
		    case '{':
			braceCount++;
			nestingLevel++;
			break;
		    case '}':
			braceCount++;
			nestingLevel--;
			break;
		    case ']':
		    case '[':
			curFlags |= WANT_PARENS;
			brackets++;
			break;
		    case '$':
			curFlags |= WANT_PARENS;
			dollars++;
			break;
		    case ' ':
		    case '\n':
		    case '\t':
			curFlags |= WANT_PARENS;
			whiteSpace++;
			break;
		    case '\\':
			src++;
			if (*src == 0) {
			    goto elementDone;
			} else if ((*src == '{') || (*src == '}')
				|| (*src == '[') || (*src == ']')) {
			    curFlags |= WANT_PARENS;
			    nestedBS++;
			}
			break;
		    case 0:
			goto elementDone;
		}
	    }
	}
	elementDone:
	numChars += src - argv[i];
	if (nestingLevel != 0) {
	    numChars += braceCount + nestedBS + whiteSpace
		    + brackets + dollars;
	    curFlags = CANT_PARENTHESIZE;
	}
	if (curFlags & WANT_PARENS) {
	    numChars += 2;
	}
	numChars++;		/* Space to separate arguments. */
	flagPtr[i] = curFlags;
    }

    /*
     * Pass two: copy into the result area.
     */

    result = (char *) malloc((unsigned) numChars + 1);
    dst = result;
    for (i = 0; i < argc; i++) {
	curFlags = flagPtr[i];
	if (curFlags & WANT_PARENS) {
	    *dst = '{';
	    dst++;
	}
	for (src = argv[i]; *src != 0 ; src++) {
	    if (curFlags & CANT_PARENTHESIZE) {
		switch (*src) {
		    case '{':
		    case '}':
		    case ']':
		    case '[':
		    case '$':
		    case ' ':
			*dst = '\\';
			dst++;
			break;
		    case '\n':
			*dst = '\\';
			dst++;
			*dst = 'n';
			goto loopBottom;
		    case '\t':
			*dst = '\\';
			dst++;
			*dst = 't';
			goto loopBottom;
		    case '\\':
			*dst = '\\';
			dst++;
			src++;
			if ((*src == '{') || (*src == '}') || (*src == '[')
				|| (*src == ']')) {
			    *dst = '\\';
			    dst++;
			} else if (*src == 0) {
			    goto pass2ElementDone;
			}
			break;
		}
	    }
	    *dst = *src;
	    loopBottom:
	    dst++;
	}
	pass2ElementDone:
	if (curFlags & WANT_PARENS) {
	    *dst = '}';
	    dst++;
	}
	*dst = ' ';
	dst++;
    }
    if (dst == result) {
	*dst = 0;
    } else {
	dst[-1] = 0;
    }

    if (flagPtr != localFlags) {
	free((char *) flagPtr);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_Concat --
 *
 *	Concatenate a set of strings into a single large string.
 *
 * Results:
 *	The return value is dynamically-allocated string containing
 *	a concatenation of all the strings in argv, with spaces between
 *	the original argv elements.
 *
 * Side effects:
 *	Memory is allocated for the result;  the caller is responsible
 *	for freeing the memory.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_Concat(argc, argv)
    int argc;			/* Number of strings to concatenate. */
    char **argv;		/* Array of strings to concatenate. */
{
    int totalSize, i;
    register char *p;
    char *result;

    for (totalSize = 1, i = 0; i < argc; i++) {
	totalSize += strlen(argv[i]) + 1;
    }
    result = malloc((unsigned) totalSize);
    for (p = result, i = 0; i < argc; i++) {
	(void) strcpy(p, argv[i]);
	p += strlen(argv[i]);
	*p = ' ';
	p++;
    }
    p[-1] = 0;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_Return --
 *
 *	Arrange for "string" to be the Tcl return value.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	interp->result is left pointing either to "string" (if "copy" is 0)
 *	or to a copy of string.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_Return(interp, string, status)
    Tcl_Interp *interp;		/* Interpreter with which to associate the
				 * return value. */
    char *string;		/* Value to be returned.  If NULL,
				 * the result is set to an empty string. */
    int status;			/* Gives information about the string:
				 * TCL_STATIC, TCL_DYNAMIC, TCL_VOLATILE.
				 * Ignored if string is NULL. */
{
    register Interp *iPtr = (Interp *) interp;
    int length;
    int wasDynamic = iPtr->dynamic;
    char *oldResult = iPtr->result;

    if (string == NULL) {
	iPtr->resultSpace[0] = 0;
	iPtr->result = iPtr->resultSpace;
	iPtr->dynamic = 0;
    } else if (status == TCL_STATIC) {
	iPtr->result = string;
	iPtr->dynamic = 0;
    } else if (status == TCL_DYNAMIC) {
	iPtr->result = string;
	iPtr->dynamic = 1;
    } else {
	length = strlen(string);
	if (length > TCL_RESULT_SIZE) {
	    iPtr->result = (char *) malloc((unsigned) length+1);
	    iPtr->dynamic = 1;
	} else {
	    iPtr->result = iPtr->resultSpace;
	    iPtr->dynamic = 0;
	}
	strcpy(iPtr->result, string);
    }

    /*
     * If the old result was dynamically-allocated, free it up.  Do it
     * here, rather than at the beginning, in case the new result value
     * was part of the old result value.
     */

    if (wasDynamic) {
	free(oldResult);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_Backslash --
 *
 *	Figure out how to handle a backslash sequence.
 *
 * Results:
 *	The return value is the character that should be substituted
 *	in place of the backslash sequence that starts at src.  If
 *	readPtr isn't NULL then it is filled in with a count of the
 *	number of characters in the backslash sequence.  Note:  if
 *	the backslash isn't followed by characters that are understood
 *	here, then the backslash sequence is only considered to be
 *	one character long, and it is replaced by a backslash char.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char
Tcl_Backslash(src, readPtr)
    char *src;			/* Points to the backslash character of
				 * a backslash sequence. */
    int *readPtr;		/* Fill in with number of characters read
				 * from src, unless NULL. */
{
    register char *p = src+1;
    char result;
    int count;

    count = 2;

    switch (*p) {
	case 'b':
	    result = '\b';
	    break;
	case 'e':
	    result = 033;
	    break;
	case 'n':
	    result = '\n';
	    break;
	case 'r':
	    result = '\r';
	    break;
	case 't':
	    result = '\t';
	    break;
	case 'C':
	    p++;
	    if (isspace(*p) || (*p == 0)) {
		result = 'C';
		count = 1;
		break;
	    }
	    count = 3;
	    if (*p == 'M') {
		p++;
		if (isspace(*p) || (*p == 0)) {
		    result = 'M' & 037;
		    break;
		}
		count = 4;
		result = (*p & 037) | 0200;
		break;
	    }
	    count = 3;
	    result = *p & 037;
	    break;
	case 'M':
	    p++;
	    if (isspace(*p) || (*p == 0)) {
		result = 'M';
		count = 1;
		break;
	    }
	    count = 3;
	    result = *p + 0200;
	    break;
	case '}':
	case '{':
	case ']':
	case '[':
	case '$':
	case ' ':
	case ';':
	case '"':
	case '\\':
	    result = *p;
	    break;
	default:
	    if (isdigit(*p)) {
		result = *p - '0';
		p++;
		if (!isdigit(*p)) {
		    break;
		}
		count = 3;
		result = (result << 3) + (*p - '0');
		p++;
		if (!isdigit(*p)) {
		    break;
		}
		count = 4;
		result = (result << 3) + (*p - '0');
		break;
	    }
	    result = '\\';
	    count = 1;
	    break;
    }

    if (readPtr != NULL) {
	*readPtr = count;
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SplitList --
 *
 *	Splits a list up into its constituent fields.
 *
 * Results
 *	The return value is normally TCL_OK, which means that
 *	the list was successfully split up.  If TCL_ERROR is
 *	returned, it means that "list" didn't have proper list
 *	structure;  interp->result will contain a more detailed
 *	error message.
 *
 *	*argvPtr will be filled in with the address of an array
 *	whose elements point to the elements of list, in order.
 *	*argcPtr will get filled in with the number of valid elements
 *	in the array.  A single block of memory is dynamically allocated
 *	to hold both the argv array and a copy of the list (with
 *	backslashes and braces removed in the standard way).
 *	The caller must eventually free this memory by calling free()
 *	on *argvPtr.  Note:  *argvPtr and *argcPtr are only modified
 *	if the procedure returns normally.
 *
 * Side effects:
 *	Memory is allocated.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_SplitList(interp, list, argcPtr, argvPtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    char *list;			/* Pointer to string with list structure. */
    int *argcPtr;		/* Pointer to location to fill in with
				 * the number of elements in the list. */
    char ***argvPtr;		/* Pointer to place to store pointer to array
				 * of pointers to list elements. */
{
    char **argv;
    register char *p;
    int size, i, result, elSize, brace;
    char *element;

    /*
     * Figure out how much space to allocate.  There must be enough
     * space for both the array of pointers and also for a copy of
     * the list.  To estimate the number of pointers needed, count
     * the number of space characters in the list.
     */

    for (size = 1, p = list; *p != 0; p++) {
	if (isspace(*p)) {
	    size++;
	}
    }
    argv = (char **) malloc((unsigned)
	    ((size * sizeof(char *)) + (p - list) + 1));
    for (i = 0, p = ((char *) argv) + size*sizeof(char *);
	    *list != 0; i++) {
	result = TclFindElement(interp, list, &element, &list, &elSize, &brace);
	if (result != TCL_OK) {
	    free((char *) argv);
	    return result;
	}
	if (*element == 0) {
	    break;
	}
	if (i >= size) {
	    Tcl_Return(interp, "internal error in Tcl_SplitList", TCL_STATIC);
	    return TCL_ERROR;
	}
	argv[i] = p;
	if (brace) {
	    strncpy(p, element, elSize);
	    p += elSize;
	    *p = 0;
	    p++;
	} else {
	    TclCopyAndCollapse(elSize, element, p);
	    p += elSize+1;
	}
    }

    *argvPtr = argv;
    *argcPtr = i;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_StringMatch --
 *
 *	See if a particular string matches a particular pattern.
 *
 * Results:
 *	The return value is 1 if string matches pattern, and
 *	0 otherwise.  The matching operation permits the following
 *	special characters in the pattern: *?\[] (see the manual
 *	entry for details on what these mean).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_StringMatch(string, pattern)
    register char *string;	/* String. */
    register char *pattern;	/* Pattern, which may contain
				 * special characters. */
{
    char c2;

    while (1) {
	/* See if we're at the end of both the pattern and the string.
	 * If so, we succeeded.  If we're at the end of the pattern
	 * but not at the end of the string, we failed.
	 */
	
	if (*pattern == 0) {
	    if (*string == 0) {
		return 1;
	    } else {
		return 0;
	    }
	}
	if ((*string == 0) && (*pattern != '*')) {
	    return 0;
	}

	/* Check for a "*" as the next pattern character.  It matches
	 * any substring.  We handle this by calling ourselves
	 * recursively for each postfix of string, until either we
	 * match or we reach the end of the string.
	 */
	
	if (*pattern == '*') {
	    pattern += 1;
	    if (*pattern == 0) {
		return 1;
	    }
	    while (*string != 0) {
		if (Tcl_StringMatch(string, pattern)) {
		    return 1;
		}
		string += 1;
	    }
	    return 0;
	}
    
	/* Check for a "?" as the next pattern character.  It matches
	 * any single character.
	 */

	if (*pattern == '?') {
	    goto thisCharOK;
	}

	/* Check for a "[" as the next pattern character.  It is followed
	 * by a list of characters that are acceptable, or by a range
	 * (two characters separated by "-").
	 */
	
	if (*pattern == '[') {
	    pattern += 1;
	    while (1) {
		if ((*pattern == ']') || (*pattern == 0)) {
		    return 0;
		}
		if (*pattern == *string) {
		    break;
		}
		if (pattern[1] == '-') {
		    c2 = pattern[2];
		    if (c2 == 0) {
			return 0;
		    }
		    if ((*pattern <= *string) && (c2 >= *string)) {
			break;
		    }
		    if ((*pattern >= *string) && (c2 <= *string)) {
			break;
		    }
		    pattern += 2;
		}
		pattern += 1;
	    }
	    while ((*pattern != ']') && (*pattern != 0)) {
		pattern += 1;
	    }
	    goto thisCharOK;
	}
    
	/* If the next pattern character is '/', just strip off the '/'
	 * so we do exact matching on the character that follows.
	 */
	
	if (*pattern == '\\') {
	    pattern += 1;
	    if (*pattern == 0) {
		return 0;
	    }
	}

	/* There's no special character.  Just make sure that the next
	 * characters of each string match.
	 */
	
	if (*pattern != *string) {
	    return 0;
	}

	thisCharOK: pattern += 1;
	string += 1;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TclWordEnd --
 *
 *	Given a pointer into a Tcl command, find the end of the next
 *	word of the command.
 *
 * Results:
 *	The return value is a pointer to the character just after the
 *	last one that's part of the word pointed to by "start".  This
 *	may be the address of the NULL character at the end of the
 *	string.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
TclWordEnd(start, nested)
    char *start;		/* Beginning of a word of a Tcl command. */
    int nested;			/* Zero means this is a top-level command.
				 * One means this is a nested command (close
				 * brace is a word terminator). */
{
    register char *p;
    int count;

    p = start;
    while (isspace(*p)) {
	p++;
    }

    /*
     * Handle words beginning with a double-quote or a brace.
     */

    if (*p == '"') {
	while (1) {
	    p++;
	    while (*p == '\\') {
		(void) Tcl_Backslash(p, &count);
		p += count;
	    }
	    if (*p == '"') {
		break;
	    }
	}
    } else if (*p == '{') {
	int braces = 1;
	while (braces != 0) {
	    p++;
	    while (*p == '\\') {
		(void) Tcl_Backslash(p, &count);
		p += count;
	    }
	    if (*p == '}') {
		braces--;
	    } else if (*p == '{') {
		braces++;
	    } else if (*p == 0) {
		return p;
	    }
	}
    }

    /*
     * Handle words that don't start with a brace or double-quote.
     * This code is also invoked if the word starts with a brace or
     * double-quote and there is garbage after the closing brace or
     * quote.  This is an error as far as Tcl_Eval is concerned, but
     * for here the garbage is treated as part of the word.
     */

    while (1) {

	/*
	 * Handle nested commands.
	 */

	while (*p == '[') {
	    p++;
	    while ((*p != ']') && (*p != 0)) {
		p = TclWordEnd(p, 1);
		if (*p == ';') {
		    p++;
		}
	    }
	    if (*p == ']') {
		p++;
	    }
	}

	/*
	 * Handle backslash sequences.  Backslash-newline isn't handled
	 * by Tcl_Backslash, so it must be checked for explicitly.
	 */

	while (*p == '\\') {
	    if (p[1] == '\n') {
		p += 2;
	    } else {
		(void) Tcl_Backslash(p, &count);
		p += count;
	    }
	}

	/*
	 * Check for end of word.  Note:  semi-colon terminates a word
	 * and also counts as a word by itself.
	 */

	if (*p == ';') {
	    if (p == start) {
		p++;
	    }
	    break;
	}
	if (isspace(*p) || (*p == 0)) {
	    break;
	}
	if ((*p == ']') && nested) {
	    break;
	}

	p++;
    }
    return p;
}
