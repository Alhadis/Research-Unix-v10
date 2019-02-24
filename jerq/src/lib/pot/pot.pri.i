# 1 "pot.pri.t"

/* <<AT&T C++ Translator 2.1++ 08/24/90>> */
/* < pot.pri.t > */

# 1 "pot.pri.t"
char *__vec_new ();

# 1 "pot.pri.t"
char __vec_delete ();
typedef int (*__vptp)();
struct __mptr {short d; short i; __vptp f; };

# 1 "pot.pri.t"

# 31416 "/usr/jerq/include/CC/jerq.h"

# 31438 "/usr/jerq/include/CC/jerq.h"
typedef int Word ;

# 31440 "/usr/jerq/include/CC/jerq.h"
typedef unsigned int UWord ;

# 31444 "/usr/jerq/include/CC/jerq.h"
struct Point {	/* sizeof Point == 4 */

# 31446 "/usr/jerq/include/CC/jerq.h"
short x__5Point ;

# 31446 "/usr/jerq/include/CC/jerq.h"
short y__5Point ;
};

# 1 "pot.pri.t"
extern char *__nw__FUi ();

# 31448 "/usr/jerq/include/CC/jerq.h"

# 31469 "/usr/jerq/include/CC/jerq.h"
struct Rectangle {	/* sizeof Rectangle == 8 */

# 31471 "/usr/jerq/include/CC/jerq.h"
struct Point o__9Rectangle ;

# 31471 "/usr/jerq/include/CC/jerq.h"
struct Point c__9Rectangle ;
};

# 31447 "/usr/jerq/include/CC/jerq.h"

# 31450 "/usr/jerq/include/CC/jerq.h"

# 31473 "/usr/jerq/include/CC/jerq.h"

# 31450 "/usr/jerq/include/CC/jerq.h"
static struct Point __pl__5PointF5Point ();

# 31473 "/usr/jerq/include/CC/jerq.h"

# 31451 "/usr/jerq/include/CC/jerq.h"
static struct Point __mi__5PointF5Point ();

# 31473 "/usr/jerq/include/CC/jerq.h"

# 31453 "/usr/jerq/include/CC/jerq.h"

# 31462 "/usr/jerq/include/CC/jerq.h"

# 31463 "/usr/jerq/include/CC/jerq.h"

# 31460 "/usr/jerq/include/CC/jerq.h"

# 31461 "/usr/jerq/include/CC/jerq.h"

# 31465 "/usr/jerq/include/CC/jerq.h"

# 31464 "/usr/jerq/include/CC/jerq.h"
static int __lt__5PointF9Rectangle ();
static int __le__5PointF9Rectangle ();

# 31491 "/usr/jerq/include/CC/jerq.h"
struct Bitmap {	/* sizeof Bitmap == 20 */

# 31493 "/usr/jerq/include/CC/jerq.h"
int *base__6Bitmap ;
unsigned int width__6Bitmap ;
struct Rectangle rect__6Bitmap ;
char *_null__6Bitmap ;
};

# 31472 "/usr/jerq/include/CC/jerq.h"

# 31523 "/usr/jerq/include/CC/jerq.h"
struct Mouse {	/* sizeof Mouse == 6 */

# 31525 "/usr/jerq/include/CC/jerq.h"
struct Point xy__5Mouse ;
short buttons__5Mouse ;
};

# 31447 "/usr/jerq/include/CC/jerq.h"

# 31527 "/usr/jerq/include/CC/jerq.h"
extern struct Mouse mouse ;

# 31552 "/usr/jerq/include/CC/jerq.h"
extern Word topbits [];

# 31552 "/usr/jerq/include/CC/jerq.h"
extern Word botbits [];
extern struct Rectangle Jrect ;
extern struct Bitmap display ;

# 31559 "/usr/jerq/include/CC/jerq.h"
typedef int Code ;

# 30 "pot.pri.t"
extern struct Pot *base ;

# 10 "pot.pri.t"
struct Pot {	/* sizeof Pot == 28 */
short flags__3Pot ;
short pad__3Pot ;
struct Pot *next__3Pot ;
struct Rectangle r__3Pot ;

# 24 "pot.pri.t"
short val__3Pot ;
char *help__3Pot ;

# 29 "pot.pri.t"
struct __mptr *__vptr__3Pot ;
};

# 31465 "/usr/jerq/include/CC/jerq.h"
extern struct __mptr* __ptbl_vec__pot_pri_t_[];
static int __le__5PointF9Rectangle (__0this , __0r )
# 31466 "/usr/jerq/include/CC/jerq.h"
register struct Point *__0this ;

# 31466 "/usr/jerq/include/CC/jerq.h"
struct Rectangle __0r ;

# 31485 "/usr/jerq/include/CC/jerq.h"
{ 
# 31485 "/usr/jerq/include/CC/jerq.h"
struct Point __0__Xp00as4lgf ;

# 31485 "/usr/jerq/include/CC/jerq.h"
struct Point __0__Xp00e54lgf ;

# 31485 "/usr/jerq/include/CC/jerq.h"
return (( (__0__Xp00as4lgf = __0r . o__9Rectangle ), ( ((__0this -> x__5Point >= __0__Xp00as4lgf . x__5Point )&& (__0this -> y__5Point >= __0__Xp00as4lgf . y__5Point )))
# 31485 "/usr/jerq/include/CC/jerq.h"
) && ( (__0__Xp00e54lgf = __0r . c__9Rectangle ), ( ((__0this -> x__5Point <= __0__Xp00e54lgf . x__5Point )&& (__0this -> y__5Point <= __0__Xp00e54lgf .
# 31485 "/usr/jerq/include/CC/jerq.h"
y__5Point ))) ) );

# 31485 "/usr/jerq/include/CC/jerq.h"
}

# 31464 "/usr/jerq/include/CC/jerq.h"
static int __lt__5PointF9Rectangle (__0this , __0r )
# 31464 "/usr/jerq/include/CC/jerq.h"
register struct Point *__0this ;

# 31464 "/usr/jerq/include/CC/jerq.h"
struct Rectangle __0r ;

# 31484 "/usr/jerq/include/CC/jerq.h"
{ 
# 31484 "/usr/jerq/include/CC/jerq.h"
struct Point __0__Xp00im5lfd ;

# 31484 "/usr/jerq/include/CC/jerq.h"
struct Point __0__Xp00mz5lfd ;

# 31484 "/usr/jerq/include/CC/jerq.h"
return (( (__0__Xp00im5lfd = __0r . o__9Rectangle ), ( ((__0this -> x__5Point > __0__Xp00im5lfd . x__5Point )&& (__0this -> y__5Point > __0__Xp00im5lfd . y__5Point )))
# 31484 "/usr/jerq/include/CC/jerq.h"
) && ( (__0__Xp00mz5lfd = __0r . c__9Rectangle ), ( ((__0this -> x__5Point < __0__Xp00mz5lfd . x__5Point )&& (__0this -> y__5Point < __0__Xp00mz5lfd .
# 31484 "/usr/jerq/include/CC/jerq.h"
y__5Point ))) ) );

# 31484 "/usr/jerq/include/CC/jerq.h"
}

# 31450 "/usr/jerq/include/CC/jerq.h"
static struct Point __pl__5PointF5Point (__0this , __0p )
# 31450 "/usr/jerq/include/CC/jerq.h"
register struct Point *__0this ;

# 31450 "/usr/jerq/include/CC/jerq.h"
struct Point __0p ;

# 31450 "/usr/jerq/include/CC/jerq.h"
{ 
# 31450 "/usr/jerq/include/CC/jerq.h"
struct Point __0__X__V3600m5zlil ;

# 31450 "/usr/jerq/include/CC/jerq.h"
{ return ( ( (( ((& __0__X__V3600m5zlil )-> x__5Point = (__0this -> x__5Point + __0p . x__5Point )), ((& __0__X__V3600m5zlil )->
# 31450 "/usr/jerq/include/CC/jerq.h"
y__5Point = (__0this -> y__5Point + __0p . y__5Point ))) ), (((& __0__X__V3600m5zlil )))) , __0__X__V3600m5zlil ) ;
} 
# 31450 "/usr/jerq/include/CC/jerq.h"
}
static struct Point __mi__5PointF5Point (__0this , __0p )
# 31451 "/usr/jerq/include/CC/jerq.h"
register struct Point *__0this ;

# 31451 "/usr/jerq/include/CC/jerq.h"
struct Point __0p ;

# 31451 "/usr/jerq/include/CC/jerq.h"
{ 
# 31451 "/usr/jerq/include/CC/jerq.h"
struct Point __0__X__V3500uw0ljj ;

# 31451 "/usr/jerq/include/CC/jerq.h"
{ return ( ( (( ((& __0__X__V3500uw0ljj )-> x__5Point = (__0this -> x__5Point - __0p . x__5Point )), ((& __0__X__V3500uw0ljj )->
# 31451 "/usr/jerq/include/CC/jerq.h"
y__5Point = (__0this -> y__5Point - __0p . y__5Point ))) ), (((& __0__X__V3500uw0ljj )))) , __0__X__V3500uw0ljj ) ;
} 
# 31451 "/usr/jerq/include/CC/jerq.h"
}

# 108 "pot.pri.t"

/* the end */
