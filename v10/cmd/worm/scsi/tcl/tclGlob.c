/* 
 * tclGlob.c --
 *
 *	This file provides procedures and commands for file name
 *	manipulation, such as tilde expansion and globbing.
 *
 * Copyright 1990 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/tcl/RCS/tclGlob.c,v 1.4 90/04/19 14:53:59 ouster Exp $ SPRITE (Berkeley)";
#pragma ref rcsid
#endif not lint

#define	_POSIX_SOURCE

#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <tcl.h>
#include "tclInt.h"

/*
 * The structure below is used to keep track of a globbing result
 * being built up (i.e. a partial list of file names).  The list
 * grows dynamically to be as big as needed.
 */

typedef struct {
    char *result;		/* Pointer to result area. */
    int totalSpace;		/* Total number of characters allocated
				 * for result. */
    int spaceUsed;		/* Number of characters currently in use
				 * to hold the partial result (not including
				 * the terminating NULL). */
    int dynamic;		/* 0 means result is static space, 1 means
				 * it's dynamic. */
} GlobResult;

/*
 *----------------------------------------------------------------------
 *
 * AppendResult --
 *
 *	Given two parts of a file name (directory and element within
 *	directory), concatenate the two together and add them to a
 *	partially-formed result.
 *
 * Results:
 *	There is no return value.  The structure at *resPtr is modified
 *	to hold more information.
 *
 * Side effects:
 *	Storage may be allocated if we run out of space in *resPtr.
 *
 *----------------------------------------------------------------------
 */

static void
AppendResult(dir, name, nameLength, resPtr)
    char *dir;			/* Name of directory (without trailing
				 * slash). */
    char *name;			/* Name of file withing directory (NOT
				 * necessarily null-terminated!). */
    int nameLength;		/* Number of characters in name. */
    register GlobResult *resPtr;/* Structure in which to append info. */
{
    int dirLength, totalLength;
    char *p;

    /*
     * Make sure there's enough space in the result area for this
     * new name (need two extra chars. besides what's in dir and
     * name, for a separating space after the last name and for a
     * terminating NULL).
     */

    dirLength = strlen(dir);
    totalLength = resPtr->spaceUsed + dirLength + nameLength + 2;
    if (totalLength > resPtr->totalSpace) {
	char *newSpace;
	int newSize;

	newSize = 2*resPtr->totalSpace;
	if (newSize < totalLength) {
	    newSize = totalLength;
	}
	newSpace = malloc((unsigned) newSize);
	bcopy(resPtr->result, newSpace, resPtr->spaceUsed);
	if (resPtr->dynamic) {
	    free(resPtr->result);
	}
	resPtr->result = newSpace;
	resPtr->totalSpace = newSize;
	resPtr->dynamic = 1;
    }

    /*
     * Now append the new information onto the end of the result.
     */

    p = resPtr->result + resPtr->spaceUsed;
    if (resPtr->spaceUsed != 0) {
	*p = ' ';
	p++;
	resPtr->spaceUsed++;
    }
    strcpy(p, dir);
    p += dirLength;
    strncpy(p, name, nameLength);
    p[nameLength] = 0;
    resPtr->spaceUsed += nameLength+dirLength;
}

/*
 *----------------------------------------------------------------------
 *
 * DoGlob --
 *
 *	This recursive procedure forms the heart of the globbing
 *	code.  It performs a depth-first traversal of the tree
 *	given by the path name to be globbed.
 *
 * Results:
 *	The return value is a standard Tcl result indicating whether
 *	an error occurred in globbing.  The result in interp will be
 *	set to hold an error message, if any.  The result pointed
 *	to by resPtr is updated to hold all file names given by
 *	the dir and rem arguments.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
DoGlob(interp, dir, rem, resPtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting (e.g. unmatched brace). */
    char *dir;				/* Name of a directory at which to
					 * start glob expansion.  This name
					 * is fixed: it doesn't contain any
					 * globbing chars.  If it's non-empty
					 * then it should end with a slash. */
    char *rem;				/* Path to glob-expand. */
    GlobResult *resPtr;			/* Where to store fully-expanded file
					 * names.*/
{
    /*
     * When this procedure is entered, the name to be globbed may
     * already have been partly expanded by ancestor invocations of
     * DoGlob.  The part that's already been expanded is in "dir"
     * (this may initially be empty), and the part still to expand
     * is in "rem".  This procedure expands "rem" one level, making
     * recursive calls to itself if there's still more stuff left
     * in the remainder.
     */

    register char *p;
    register char c;
    char *openBrace, *closeBrace;
    int gotSpecial, result;

    /*
     * When generating information for the next lower call,
     * use static areas if the name is short, and malloc if the name
     * is longer.
     */

#define STATIC_SIZE 200

    /*
     * First, find the end of the next element in rem, checking
     * along the way for special globbing characters.
     */

    gotSpecial = 0;
    openBrace = closeBrace = NULL;
    for (p = rem; ; p++) {
	c = *p;
	if ((c == '\0') || (c == '/')) {
	    break;
	}
	if ((c == '{') && (openBrace == NULL)) {
	    openBrace = p;
	}
	if ((c == '}') && (closeBrace == NULL)) {
	    closeBrace = p;
	}
	if ((c == '*') || (c == '[') || (c == '\\') || (c == '?')) {
	    gotSpecial = 1;
	}
    }

    /*
     * If there is an open brace in the argument, then make a recursive
     * call for each element between the braces.  In this case, the
     * recursive call to DoGlob uses the same "dir" that we got.
     * If there are several brace-pairs in a single name, we just handle
     * one here, and the others will be handled in recursive calls.
     */

    if (openBrace != NULL) {
	int remLength, l1, l2;
	char static1[STATIC_SIZE];
	char *element, *newRem;

	if (closeBrace == NULL) {
	    interp->result = "unmatched open-brace in file name";
	    return TCL_ERROR;
	}
	remLength = strlen(rem) + 1;
	if (remLength <= STATIC_SIZE) {
	    newRem = static1;
	} else {
	    newRem = malloc((unsigned) remLength);
	}
	l1 = openBrace-rem;
	strncpy(newRem, rem, l1);
	p = openBrace;
	for (p = openBrace; *p != '}'; ) {
	    element = p+1;
	    for (p = element; ((*p != '}') && (*p != ',')); p++) {
		/* Empty body:  just find end of this element. */
	    }
	    l2 = p - element;
	    strncpy(newRem+l1, element, l2);
	    strcpy(newRem+l1+l2, closeBrace+1);
	    if (DoGlob(interp, dir, newRem, resPtr) != TCL_OK) {
		return TCL_ERROR;
	    }
	}
	if (remLength > STATIC_SIZE) {
	    free(newRem);
	}
	return TCL_OK;
    }

    /*
     * If there were any pattern-matching characters, then scan through
     * the directory to find all the matching names.
     */

    if (gotSpecial) {
	DIR *d;
	struct dirent *entryPtr;
	int l1, l2;
	char *pattern, *newDir;
	char static1[STATIC_SIZE], static2[STATIC_SIZE];
	struct stat statBuf;

	if ((stat(dir, &statBuf) != 0)
		|| !S_ISDIR(statBuf.st_mode)) {
	    return TCL_OK;
	}
	d = opendir(dir);
	if (d == NULL) {
	    sprintf(interp->result,
		    "couldn't read directory \"%.50s\": %.50s",
		    dir, strerror(errno));
	    return TCL_ERROR;
	}
	l1 = strlen(dir);
	l2 = (p - rem);
	if (l2 < STATIC_SIZE) {
	    pattern = static2;
	} else {
	    pattern = malloc((unsigned) (l2+1));
	}
	strncpy(pattern, rem, l2);
	pattern[l2] = '\0';
	result = TCL_OK;
	while (1) {
	    entryPtr = readdir(d);
	    if (entryPtr == NULL) {
		break;
	    }

	    /*
	     * Don't match names starting with "." unless the "." is
	     * present in the pattern.
	     */

	    if ((*entryPtr->d_name == '.') && (*pattern != '.')) {
		continue;
	    }
	    if (Tcl_StringMatch(entryPtr->d_name, pattern)) {
		if (*p == 0) {
		    AppendResult(dir, entryPtr->d_name,
			    (int) entryPtr->d_namlen, resPtr);
		} else {
		    if ((l1+entryPtr->d_namlen+2) <= STATIC_SIZE) {
			newDir = static1;
		    } else {
			newDir = malloc((unsigned) (l1+entryPtr->d_namlen+2));
		    }
		    sprintf(newDir, "%s%s/", dir, entryPtr->d_name);
		    result = DoGlob(interp, newDir, p+1, resPtr);
		    if (newDir != static1) {
			free(newDir);
		    }
		    if (result != TCL_OK) {
			break;
		    }
		}
	    }
	}
	if (pattern != static2) {
	    free(pattern);
	}
	return result;
    }

    /*
     * This is the simplest case:  just another path element.  Move
     * it to the dir side and recurse (or just add the name to the
     * list, if we're at the end of the path).
     */

    if (*p == 0) {
	AppendResult(dir, rem, p-rem, resPtr);
    } else {
	int l1, l2;
	char *newDir;
	char static1[STATIC_SIZE];

	l1 = strlen(dir);
	l2 = l1 + (p - rem) + 2;
	if (l2 <= STATIC_SIZE) {
	    newDir = static1;
	} else {
	    newDir = malloc((unsigned) l2);
	}
	strcpy(newDir, dir);
	strncpy(newDir+l1, rem, p-rem);
	newDir[l2-2] = '/';
	newDir[l2-1] = 0;
	result = DoGlob(interp, newDir, p+1, resPtr);
	if (newDir != static1) {
	    free(newDir);
	}
	if (result != TCL_OK) {
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TildeSubst --
 *
 *	Given a name starting with a tilde, produce a name where
 *	the tilde and following characters have been replaced by
 *	the home directory location for the named user.
 *
 * Results:
 *	The result is a pointer to a static string containing
 *	the new name.  This name will only persist until the next
 *	call to Tcl_TildeSubst;  save it if you care about it for
 *	the long term.  If there was an error in processing the
 *	tilde, then an error message is left in interp->result
 *	and the return value is NULL.
 *
 * Side effects:
 *	None that the caller needs to worry about.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_TildeSubst(interp, name)
    Tcl_Interp *interp;		/* Interpreter in which to store error
				 * message (if necessary). */
    char *name;			/* File name, which may begin with "~/"
				 * (to indicate current user's home directory)
				 * or "~<user>/" (to indicate any user's
				 * home directory). */
{
#define STATIC_BUF_SIZE 50
    static char staticBuf[STATIC_BUF_SIZE];
    static int curSize = STATIC_BUF_SIZE;
    static char *curBuf = staticBuf;
    char *dir;
    int length;
    int fromPw = 0;
    register char *p;

    if (name[0] != '~') {
	return name;
    }

    /*
     * First, find the directory name corresponding to the tilde entry.
     */

    if ((name[1] == '/') || (name[1] == '\0')) {
	dir = getenv("HOME");
	if (dir == NULL) {
	    sprintf(interp->result,
		    "couldn't find HOME env. variable to expand \"%.100s\"",
		    name);
	    return NULL;
	}
	p = name+1;
    } else {
	struct passwd *pwPtr;

	for (p = &name[1]; (*p != 0) && (*p != '/'); p++) {
	    /* Null body;  just find end of name. */
	}
	length = p-&name[1];
	if (length >= curSize) {
	    length = curSize-1;
	}
	bcopy(name+1, curBuf, length);
	curBuf[length] = '\0';
	pwPtr = getpwnam(curBuf);
	if (pwPtr == NULL) {
	    sprintf(interp->result, "user \"%.50s\" doesn't exist", curBuf);
	    return NULL;
	}
	dir = pwPtr->pw_dir;
	fromPw = 1;
    }

    /*
     * Grow the buffer if necessary to make enough space for the
     * full file name.
     */

    length = strlen(dir) + strlen(p);
    if (length >= curSize) {
	if (curBuf != staticBuf) {
	    free(curBuf);
	}
	curSize = length + 1;
	curBuf = malloc((unsigned) curSize);
    }

    /*
     * Finally, concatenate the directory name with the remainder
     * of the path in the buffer.
     */

    strcpy(curBuf, dir);
    strcat(curBuf, p);
    if (fromPw) {
	endpwent();
    }
    return curBuf;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GlobCmd --
 *
 *	This procedure is invoked to process the "glob" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_GlobCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    GlobResult globRes;
    char staticSpace[TCL_RESULT_SIZE];
    int i, result;

    globRes.result = staticSpace;
    globRes.totalSpace = TCL_RESULT_SIZE;
    globRes.spaceUsed = 0;
    globRes.dynamic = 0;
    for (i = 1; i < argc; i++) {
	char *thisName;

	/*
	 * Do special checks for names starting at the root and for
	 * names beginning with ~.  Then let DoGlob do the rest.
	 */

	thisName = argv[i];
	if (*thisName == '~') {
	    thisName = Tcl_TildeSubst(interp, thisName);
	    if (thisName == NULL) {
		return TCL_ERROR;
	    }
	}
	if (*thisName == '/') {
	    result = DoGlob(interp, "/", thisName+1, &globRes);
	} else {
	    result = DoGlob(interp, "", thisName, &globRes);
	}
	if (result != TCL_OK) {
	    goto error;
	}
    }
    if (globRes.spaceUsed == 0) {
	sprintf(interp->result, "no files matched glob pattern(s)");
	result = TCL_ERROR;
	goto error;
    }
    if (globRes.dynamic) {
	interp->result = globRes.result;
	interp->dynamic = 1;
    } else {
	strcpy(interp->result, globRes.result);
    }
    return TCL_OK;

    error:
    if (globRes.dynamic) {
	free(globRes.result);
    }
    return result;
}
