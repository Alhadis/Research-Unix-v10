
/* <<cfront-09/14/84>> */
/* < t.c */
int * _new ( ) ; 
int _delete ( ) ; 
int * _vec_new ( ) ; 
int _vec_delete ( ) ; 

#line 75 "/usr/jerq/include/CC/sys/2681.h"

#line 76 "/usr/jerq/include/CC/sys/2681.h"
struct duart { /* sizeof = 64 */
int _duart__F1 : 16; 
int _duart__F2 : 8; 
unsigned char _duart_mr1_2a ; 
int _duart__F3 : 16; 
int _duart__F4 : 8; 
unsigned char _duart_a_sr_csr ; 
int _duart__F5 : 16; 
int _duart__F6 : 8; 
unsigned char _duart_a_cmnd ; 
int _duart__F7 : 16; 
int _duart__F8 : 8; 
unsigned char _duart_a_data ; 
int _duart__F9 : 16; 
int _duart__F10 : 8; 
unsigned char _duart_ipc_acr ; 
int _duart__F11 : 16; 
int _duart__F12 : 8; 
unsigned char _duart_is_imr ; 
int _duart__F13 : 16; 
int _duart__F14 : 8; 
unsigned char _duart_ctur ; 
int _duart__F15 : 16; 
int _duart__F16 : 8; 
unsigned char _duart_ctlr ; 
int _duart__F17 : 16; 
int _duart__F18 : 8; 
unsigned char _duart_mr1_2b ; 
int _duart__F19 : 16; 
int _duart__F20 : 8; 
unsigned char _duart_b_sr_csr ; 
int _duart__F21 : 16; 
int _duart__F22 : 8; 
unsigned char _duart_b_cmnd ; 
int _duart__F23 : 16; 
int _duart__F24 : 8; 
unsigned char _duart_b_data ; 
int _duart__F25 : 16; 
int _duart__F26 : 8; 
int _duart__F27 : 8; 
int _duart__F28 : 16; 
int _duart__F29 : 8; 
unsigned char _duart_ip_opcr ; 
int _duart__F30 : 16; 
int _duart__F31 : 8; 
unsigned char _duart_scc_sopbc ; 
int _duart__F32 : 16; 
int _duart__F33 : 8; 
unsigned char _duart_scc_ropbc ; 
} ; 
typedef int Word ; 
typedef unsigned int UWord ; 

#line 35 "/usr/jerq/include/CC/jerq.h"
struct Point { /* sizeof = 4 */
short _Point_x ; 
short _Point_y ; 
} ; 
	/* overload _ctor: */
; 
; 
	/* overload _minus: */
; 
; 
	/* overload _le: */
; 
; 
	/* overload _gt: */
; 
; 
	/* overload _lt: */
; 
; 
static struct Point _Point__minusFCPoint___ ( ) ; 
static struct Point _Point__plus ( ) ; 

#line 60 "/usr/jerq/include/CC/jerq.h"
struct Rectangle { /* sizeof = 8 */
struct Point _Rectangle_o ; 
struct Point _Rectangle_c ; 
} ; 
	/* overload _ctor: */
; 
; 
; 
int _Rectangle__lt ( ) ; 
int _Rectangle__le ( ) ; 
	/* overload mbb: */
struct Rectangle _Rectangle_mbbFCPoint___ ( ) ; 
struct Rectangle _Rectangle_mbbFCRectangle___ ( ) ; 

#line 75 "/usr/jerq/include/CC/jerq.h"
; 

#line 76 "/usr/jerq/include/CC/jerq.h"
; 

#line 77 "/usr/jerq/include/CC/jerq.h"
; 
static int _Point__ltFCRectangle___ ( ) ; 

#line 78 "/usr/jerq/include/CC/jerq.h"
; 
static int _Point__leFCRectangle___ ( ) ; 

#line 79 "/usr/jerq/include/CC/jerq.h"
; 

#line 81 "/usr/jerq/include/CC/jerq.h"
struct Bitmap { /* sizeof = 20 */
Word * _Bitmap_base ; 
unsigned int _Bitmap_width ; 
struct Rectangle _Bitmap_rect ; 
char * _Bitmap__null ; 
} ; 

#line 90 "/usr/jerq/include/CC/jerq.h"
struct Menu { /* sizeof = 12 */
char * * _Menu_item ; 
char * ( * _Menu_generator ) ( ) ; 
short _Menu_prevhit ; 
short _Menu_prevtop ; 
} ; 

#line 98 "/usr/jerq/include/CC/jerq.h"
struct Texture32 { /* sizeof = 128 */
Word _Texture32_bits [ 32] ; 
} ; 

#line 103 "/usr/jerq/include/CC/jerq.h"
struct Texture { /* sizeof = 32 */
short _Texture_bits [ 16] ; 
} ; 

#line 109 "/usr/jerq/include/CC/jerq.h"
struct Font { /* sizeof = 4 */
short _Font_n ; 
char _Font_height ; 
char _Font_ascent ; 
} ; 

#line 115 "/usr/jerq/include/CC/jerq.h"
struct goofus { /* sizeof = 6 */
struct Point _goofus_xy ; 
short _goofus_buttons ; 
} ; 

#line 119 "/usr/jerq/include/CC/jerq.h"
extern struct goofus mouse ; 

#line 135 "/usr/jerq/include/CC/jerq.h"
extern Word topbits [ ] ; 

#line 135 "/usr/jerq/include/CC/jerq.h"
extern Word botbits [ ] ; 

#line 136 "/usr/jerq/include/CC/jerq.h"
extern struct Rectangle Jrect ; 

#line 137 "/usr/jerq/include/CC/jerq.h"
extern struct Bitmap display ; 
typedef int Code ; 

#line 7 "/usr/jerq/include/CC/mux.h"
extern struct Bitmap * Jdisplayp ; 
typedef int ( * ptr_fint ) ( ) ; 

#line 159 "/usr/jerq/include/CC/jerq.h"
extern struct Rectangle Drect ; 

#line 3 "t.c"
int main ( ) 
#line 4 "t.c"
{ 
#line 4 "t.c"
int _auto__result ; 
struct Point _auto_p ; 
struct Rectangle _auto_r ; 
struct Rectangle _auto_s ; 
struct Point _auto__Xp_translate_Rectangle ; 
struct Point _auto__Xp__minusFCPoint____Point ; 
struct Point _auto__X_V35__minusFCPoint____Point ; 
struct Point _auto__Xp__plus_Point ; 
struct Point _auto__X_V36__plus_Point ; 
struct Point _auto__Xq__ctorFCPoint__CPoint____Rectangle ; 
struct Rectangle _auto__X_V45_translate_Rectangle ; 

#line 5 "t.c"
( ( ( struct Point * ) ( & _auto_p ) ) ) ; 

#line 6 "t.c"
( ( ( ( ( ( struct Point * ) ( & ( ( struct 

#line 6 "t.c"
Rectangle * ) ( & _auto_r ) ) -> _Rectangle_o ) ) ) , ( ( ( 

#line 6 "t.c"
struct Point * ) ( & ( ( struct Rectangle * ) ( & _auto_r ) ) 

#line 6 "t.c"
-> _Rectangle_c ) ) ) ) , ( ( struct Rectangle * ) ( & _auto_r ) 

#line 6 "t.c"
) ) ) ; 

#line 6 "t.c"
( ( ( ( ( ( struct Point * ) ( & ( ( struct 

#line 6 "t.c"
Rectangle * ) ( & _auto_s ) ) -> _Rectangle_o ) ) ) , ( ( ( 

#line 6 "t.c"
struct Point * ) ( & ( ( struct Rectangle * ) ( & _auto_s ) ) 

#line 6 "t.c"
-> _Rectangle_c ) ) ) ) , ( ( struct Rectangle * ) ( & _auto_s ) 

#line 6 "t.c"
) ) ) ; 

#line 7 "t.c"
_auto_s = ( ( _auto__Xp_translate_Rectangle = _auto_p ) , ( ( ( _auto__Xq__ctorFCPoint__CPoint____Rectangle = ( ( _auto__Xp__plus_Point = ( 

#line 7 "t.c"
( _auto__Xp__minusFCPoint____Point = ( ( struct Rectangle * ) ( & _auto_r ) ) -> _Rectangle_o ) , 

#line 7 "t.c"
( ( ( ( ( ( struct Point * ) ( & _auto__X_V35__minusFCPoint____Point ) ) -> 

#line 7 "t.c"
_Point_x = ( ( ( struct Point * ) ( & _auto__Xp_translate_Rectangle ) ) -> _Point_x - _auto__Xp__minusFCPoint____Point . 

#line 7 "t.c"
_Point_x ) ) , ( ( ( ( struct Point * ) ( & _auto__X_V35__minusFCPoint____Point ) ) 

#line 7 "t.c"
-> _Point_y = ( ( ( struct Point * ) ( & _auto__Xp_translate_Rectangle ) ) -> _Point_y - 

#line 7 "t.c"
_auto__Xp__minusFCPoint____Point . _Point_y ) ) , ( ( struct Point * ) ( & _auto__X_V35__minusFCPoint____Point ) ) ) 

#line 7 "t.c"
) ) , _auto__X_V35__minusFCPoint____Point ) ) ) , ( ( ( ( ( ( struct 

#line 7 "t.c"
Point * ) ( & _auto__X_V36__plus_Point ) ) -> _Point_x = ( ( ( struct Point * ) 

#line 7 "t.c"
( & ( ( struct Rectangle * ) ( & _auto_r ) ) -> _Rectangle_c ) ) 

#line 7 "t.c"
-> _Point_x + _auto__Xp__plus_Point . _Point_x ) ) , ( ( ( ( struct Point * ) ( 

#line 7 "t.c"
& _auto__X_V36__plus_Point ) ) -> _Point_y = ( ( ( struct Point * ) ( & ( 

#line 7 "t.c"
( struct Rectangle * ) ( & _auto_r ) ) -> _Rectangle_c ) ) -> _Point_y + _auto__Xp__plus_Point . 

#line 7 "t.c"
_Point_y ) ) , ( ( struct Point * ) ( & _auto__X_V36__plus_Point ) ) ) ) 

#line 7 "t.c"
) , _auto__X_V36__plus_Point ) ) ) , ( ( ( ( ( ( struct Point * 

#line 7 "t.c"
) ( & ( ( struct Rectangle * ) ( & _auto__X_V45_translate_Rectangle ) ) -> _Rectangle_o ) 

#line 7 "t.c"
) ) , ( ( ( struct Point * ) ( & ( ( struct 

#line 7 "t.c"
Rectangle * ) ( & _auto__X_V45_translate_Rectangle ) ) -> _Rectangle_c ) ) ) ) , ( ( 

#line 7 "t.c"
( ( struct Rectangle * ) ( & _auto__X_V45_translate_Rectangle ) ) -> _Rectangle_o = _auto__Xp_translate_Rectangle ) , ( 

#line 7 "t.c"
( ( ( struct Rectangle * ) ( & _auto__X_V45_translate_Rectangle ) ) -> _Rectangle_c = _auto__Xq__ctorFCPoint__CPoint____Rectangle ) , 

#line 7 "t.c"
( ( struct Rectangle * ) ( & _auto__X_V45_translate_Rectangle ) ) ) ) ) ) ) 

#line 7 "t.c"
, _auto__X_V45_translate_Rectangle ) ) ; 
} ; 
static int _Point__leFCRectangle___ ( _auto_this , _auto_r ) register struct Point * _auto_this ; 
struct Rectangle _auto_r ; 

#line 76 "/usr/jerq/include/CC/jerq.h"
{ 
#line 76 "/usr/jerq/include/CC/jerq.h"
int _auto__result ; 
struct Point _auto__Xp__ge_Point ; 
struct Point _auto__Xp__leFCPoint____Point ; 

#line 76 "/usr/jerq/include/CC/jerq.h"
return ( ( _auto__Xp__ge_Point = _auto_r . _Rectangle_o ) , ( ( _auto_this -> _Point_x >= _auto__Xp__ge_Point . _Point_x ) && 

#line 76 "/usr/jerq/include/CC/jerq.h"
( _auto_this -> _Point_y >= _auto__Xp__ge_Point . _Point_y ) ) ) && ( ( _auto__Xp__leFCPoint____Point = _auto_r . _Rectangle_c ) , 

#line 76 "/usr/jerq/include/CC/jerq.h"
( ( _auto_this -> _Point_x <= _auto__Xp__leFCPoint____Point . _Point_x ) && ( _auto_this -> _Point_y <= _auto__Xp__leFCPoint____Point . _Point_y ) ) ) 

#line 76 "/usr/jerq/include/CC/jerq.h"
; 
} static int _Point__ltFCRectangle___ ( _auto_this , _auto_r ) register struct Point * _auto_this ; 
struct Rectangle _auto_r ; 

#line 75 "/usr/jerq/include/CC/jerq.h"
{ 
#line 75 "/usr/jerq/include/CC/jerq.h"
int _auto__result ; 
struct Point _auto__Xp__gtFCPoint____Point ; 
struct Point _auto__Xp__ltFCPoint____Point ; 

#line 75 "/usr/jerq/include/CC/jerq.h"
return ( ( _auto__Xp__gtFCPoint____Point = _auto_r . _Rectangle_o ) , ( ( _auto_this -> _Point_x > _auto__Xp__gtFCPoint____Point . _Point_x ) && 

#line 75 "/usr/jerq/include/CC/jerq.h"
( _auto_this -> _Point_y > _auto__Xp__gtFCPoint____Point . _Point_y ) ) ) && ( ( _auto__Xp__ltFCPoint____Point = _auto_r . _Rectangle_c ) , 

#line 75 "/usr/jerq/include/CC/jerq.h"
( ( _auto_this -> _Point_x < _auto__Xp__ltFCPoint____Point . _Point_x ) && ( _auto_this -> _Point_y < _auto__Xp__ltFCPoint____Point . _Point_y ) ) ) 

#line 75 "/usr/jerq/include/CC/jerq.h"
; 
} static struct Point _Point__plus ( _auto_this , _auto_p ) register struct Point * _auto_this ; 
struct Point _auto_p ; 

#line 41 "/usr/jerq/include/CC/jerq.h"
{ 
#line 41 "/usr/jerq/include/CC/jerq.h"
struct Point _auto__result ; 
struct Point _auto__X_V36__plus_Point ; 

#line 41 "/usr/jerq/include/CC/jerq.h"
{ return ( ( ( ( ( ( struct Point * ) ( & _auto__X_V36__plus_Point ) 

#line 41 "/usr/jerq/include/CC/jerq.h"
) -> _Point_x = ( _auto_this -> _Point_x + _auto_p . _Point_x ) ) , ( ( ( ( 

#line 41 "/usr/jerq/include/CC/jerq.h"
struct Point * ) ( & _auto__X_V36__plus_Point ) ) -> _Point_y = ( _auto_this -> _Point_y + _auto_p . _Point_y ) 

#line 41 "/usr/jerq/include/CC/jerq.h"
) , ( ( struct Point * ) ( & _auto__X_V36__plus_Point ) ) ) ) ) 

#line 41 "/usr/jerq/include/CC/jerq.h"
, _auto__X_V36__plus_Point ) ; 
} } static struct Point _Point__minusFCPoint___ ( _auto_this , _auto_p ) register struct Point * _auto_this ; 
struct Point _auto_p ; 

#line 42 "/usr/jerq/include/CC/jerq.h"
{ 
#line 42 "/usr/jerq/include/CC/jerq.h"
struct Point _auto__result ; 
struct Point _auto__X_V35__minusFCPoint____Point ; 

#line 42 "/usr/jerq/include/CC/jerq.h"
{ return ( ( ( ( ( ( struct Point * ) ( & _auto__X_V35__minusFCPoint____Point ) 

#line 42 "/usr/jerq/include/CC/jerq.h"
) -> _Point_x = ( _auto_this -> _Point_x - _auto_p . _Point_x ) ) , ( ( ( ( 

#line 42 "/usr/jerq/include/CC/jerq.h"
struct Point * ) ( & _auto__X_V35__minusFCPoint____Point ) ) -> _Point_y = ( _auto_this -> _Point_y - _auto_p . _Point_y ) 

#line 42 "/usr/jerq/include/CC/jerq.h"
) , ( ( struct Point * ) ( & _auto__X_V35__minusFCPoint____Point ) ) ) ) ) 

#line 42 "/usr/jerq/include/CC/jerq.h"
, _auto__X_V35__minusFCPoint____Point ) ; 
} } 
/* the end */
