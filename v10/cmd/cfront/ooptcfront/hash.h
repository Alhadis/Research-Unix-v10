/* Compiler interface to hash tables from odi library. */
/*
	$Source: /usr3/lang/benson/work/stripped_cfront/RCS/hash.h,v $ $RCSfile: hash.h,v $
	$Revision: 1.1 $		$Date: 89/11/20 08:50:36 $
	$Author: benson $		$Locker:  $
	$State: Exp $
*/

#include <hash.H>
#include <string.h>

inline int pointer_hasheq (int a, int b)
{
    return a == b;
};

inline unsigned int pointer_hash_fcn (int x)
{
    unsigned X = (unsigned) x;
    return ((X << 16) | (X >> 16)) ^ x;
}


class pointer_hash : public Hash {
  public:
    pointer_hash (int sz = 0) : Hash (sz) {
	key_hash_function = pointer_hash_fcn;
	key_key_equality_function = pointer_hasheq;
    }

    pointer_hash (pointer_hash& h) : Hash (h) {};
};

inline int string_hasheq (int a, int b)
{
    return !strcmp((char *)a, (char *) b);
};

static unsigned int string_hash_fcn (int x)
{
    char * str = (char *)x;
    int l = strlen(str);
	
    if(x <= 4) return str[0];
    else {
	unsigned int * f4 = (unsigned int *) str;
	if (l < 8) return ((*f4 << 16) | (*f4 >> 16)) ^ *f4;
	else {
	    unsigned int * s4 = f4 ++;
	    return ((*f4 << 16) | (*f4 >> 16)) ^ *s4;
	}
    }
};


class string_hash : public Hash {
    public:

    string_hash (int sz = 0) : Hash (sz) {
	key_hash_function = string_hash_fcn;
	key_key_equality_function = string_hasheq;
    };

    string_hash (string_hash& h) : Hash (h) {};
};


/*
   $Log:	hash.h,v $
 * Revision 1.1  89/11/20  08:50:36  benson
 * Initial revision
 * 
 * Revision 1.1  89/09/26  10:11:51  benson
 * Initial revision
 * 


   end_log
*/
