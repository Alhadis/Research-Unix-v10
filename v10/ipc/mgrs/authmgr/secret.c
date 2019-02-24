#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <time.h>
#include "auth.h"

#define FIELDSIZE	100	/* maximum field size in the keys file */
#define LARGECH		99999	/* largest challenge */

typedef struct keydesc keydesc;

struct keydesc {
	char	name[FIELDSIZE];
	char	type[FIELDSIZE];
	char	key[FIELDSIZE];
	long	expires;
	struct keydesc	*next;
};

struct keydesc *firstkey;
struct keydesc *kp;
enum keytypes kt;

unsigned long challenge = 0;
time_t now;			/* current time, for expiration purposes */

extern char *keyfile;

	
/*
 * Get challenge string for `login'. `type' is the login type, "" for the
 * first one found.
 */
char *
getchallenge(login, type, echo)
	char *login;
	char *type;
	int *echo;
{
	static char buf[BUFSIZ];
	static int inited=0;

	for (kp=firstkey; kp; kp = kp->next)
		if (strcmp(kp->name, login) == NULL)
			if (type == NULL || strcmp(type, "") == NULL ||
			    strcmp(type, kp->type) == NULL)
				break;

	if (kp == (keydesc *)0 || (strcmp(kp->type, "passwd") != NULL)) {
		/*
		 *  atalla entry or illegal user.  Generate a challenge.
		 */
		if(!inited)
			srand((int)time((long*)0));
		challenge = lrand()%LARGECH;
		sprintf(buf, "Enter response code for %lu: ", challenge);
		if (kp == (keydesc *)0)
			kt = NONE;
		else
			kt = ATALLA;
		*echo = TRUE;
	} else {
		kt = PASS;
		strcpy(buf, "Password: ");
		*echo = FALSE;
	}
	return buf;
}

/*
 * He gave `response'.  Return TRUE iff it is the correct
 * response.
 */
int
responseok(response)
	char response[];
{
	char b[64];
	int k[8];
	char buf[12];
	int i,j;
	unsigned long bresponse = 0;
	char cresponse[10];

	/*
	 * check expiration date.
	 */
	if (kt != NONE && kp->expires < now) {
		fprintf(stderr, "Account `%s' is expired (%d)\n",
			kp->name, kp->expires);
		kt = NONE;
		kp = (keydesc *)0;
	}

	switch(kt) {
	case NONE:	/* take some time, then say no. */
		setkey(b);
		encrypt(b, 0);
		return FALSE;
	case PASS:	/* the classic password hash */
		return (strcmp(crypt(response, kp->key), kp->key) == NULL);
	case ATALLA:
		/*
		 * set the key
		 */
		sscanf(kp->key, "%o %o %o %o %o %o %o %o",
			&k[0], &k[1], &k[2], &k[3], &k[4], &k[5], &k[6], &k[7]);
		for(i=0; i<8; i++)
			for(j=0; j<8; j++)
				b[8*i+j] = (k[i]>>(7-j))&1;
		setkey(b);

		/*
		 * compute the proper response.  We encrypt the ascii of
		 * challenge number, with trailing binary zero fill.
		 * This process was derived empirically.
		 */
		for(i=0; i<8; buf[i++] = 0)
			;
		sprintf(buf, "%lu", challenge);
		for(i=0; i<8; i++)
			for(j=0; j<8; j++)
				b[8*i+j] = (buf[i]>>(7-j))&1;
		encrypt(b, 0);
		for(i=0; i<32; i++) {
			bresponse = (bresponse<<1) | b[i];
		}

		/*
		 * check for hex match
		 */
		sprintf(cresponse, "%08x", bresponse);
		for (i=0; response[i]; i++)
			if (isupper(response[i]))
				response[i] = tolower(response[i]);
		if (strcmp(response, cresponse) == 0)
			return TRUE;	/* Hex matches */

		/*
		 * check for decimal match
		 */
		for (i=0; cresponse[i]; i++)
			if (cresponse[i] == 'a' || cresponse[i] == 'b' ||
			    cresponse[i] == 'c')
				cresponse[i] = '2';
			else if (cresponse[i] == 'd' || cresponse[i] == 'e' ||
				 cresponse[i] == 'f')
				cresponse[i] = '3';
		return (strcmp(response, cresponse) == NULL);	/* Decimal matches */
	}
}


/*
 * Read in the keys.  This won't happen in a secret-server version.
 *
 * key file has the following colon-separated fields:
 *
 *	id	user's id
 *	type	entry type (`atalla' is only legal entry at the moment)
 *	key	the key's value: 
 *		for atalla:	8 blank-separated octal bytes
 *	expire	expiration date of this entry [optional]
 *	comment	[optional]
 */
readkeyfile()
{
	FILE *kf;
	int linenumber = 0;
	char line[BUFSIZE];
#define NFLDS	10
	char *field[NFLDS];
	keydesc *lastkey = firstkey = (keydesc *)0;
	time_t t = time((long*)0);
	struct tm *tm = localtime(&t);

	now = tm->tm_mday + 100*(tm->tm_mon+1) + 10000*(tm->tm_year+1900);

	if ((kf = fopen(keyfile, "r")) == NULL) {
		printf("Could not open key file: get help!\n");
		exit(100);
	}
	setfields(":");
	while(fgets(line, sizeof(line), kf) != NULL) {
		int i;
		char *cp;

		linenumber++;
		for(i=0; line[i] == ' ' || line[i] == '\t'; i++)
			;
		if(line[i] == '#')
			continue;
		if((cp = strchr(&line[i], '\n')) != NULL)
			*cp = '\0';

		i = getfields(&line[i], field, NFLDS);
		if(i <= 1)	/* blank line */
			continue;
		if (i < 3) {
			fprintf(stderr, "Bad key entry for line %d, ignored\n",
				linenumber);
			continue;
		}

		if ((kp = (keydesc *)malloc(sizeof(keydesc))) == (keydesc *)0) {
			printf("Out of memory: get help!\n");
			fprintf("Out of memory building key table\n");
			exit(101);
		}
		if (firstkey == (keydesc *)0)
			firstkey = kp;
		if (lastkey)
			lastkey->next = kp;
		strncpy(kp->name, field[0], FIELDSIZE);
		strncpy(kp->type, field[1], FIELDSIZE);
		strncpy(kp->key,  field[2], FIELDSIZE);
		if (i >= 4)
			if(strcmp(field[3], "") != 0) {
				kp->expires = atol(field[3]);
				if (kp->expires < 19700101 ||
				    kp->expires > 99991231) {
					fprintf(stderr, "Bad expiration for line %d, ignored\n",
						linenumber);
						continue;
				}
			} else
				kp->expires = 99991231;
		kp->next = (keydesc *)0;
		lastkey = kp;
	}
	
}

dumpkeys()
{
	keydesc *kp;

	for (kp=firstkey; kp; kp = kp->next)
		printf("%s/%s/%s/%s/\n",
			kp->name, kp->type, kp->key, kp->expires);
}
