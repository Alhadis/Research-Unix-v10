struct Index { unsigned char _Index_major ; unsigned char _Index_minor ; };
struct IList { struct Index _IList_index ; struct IList * _IList_next ; };
struct Menu { struct IList * _Menu_list ; int _Menu_size ; }; 
int f( _auto_this , _auto_i )
register struct Menu * _auto_this; 
struct Index _auto_i; 
{ struct IList * _auto_l ; 
struct IList * _auto__Xthis__ctor_IList ; 
struct Index _auto__Xi__ctor_IList ; 
struct IList * _auto__Xn__ctor_IList ; 
struct Index * _auto__Xthis__ctorF__Index ; 

_auto_l->_IList_next=(struct IList*)((_auto__Xthis__ctor_IList=(
struct IList*)0),((_auto__Xi__ctor_IList=_auto_i),((_auto__Xn__ctor_IList=(struct IList*
)0),(((_auto__Xthis__ctor_IList==(struct IList*)0)?(_auto__Xthis__ctor_IList=
(struct IList*)_new((unsigned int)8)):0),((
(_auto__Xthis__ctorF__Index=(struct Index*)(&_auto__Xthis__ctor_IList->_IList_index)),(((_auto__Xthis__ctorF__Index==(
struct Index*)0)?(_auto__Xthis__ctorF__Index=(struct Index*)_new((unsigned int)
4)):0),((_auto__Xthis__ctorF__Index->_Index_major=(_auto__Xthis__ctorF__Index->_Index_minor=0)),
_auto__Xthis__ctorF__Index))),((_auto__Xthis__ctor_IList->_IList_index=_auto__Xi__ctor_IList),((_auto__Xthis__ctor_IList->_IList_next=(struct IList*
)_auto__Xn__ctor_IList),_auto__Xthis__ctor_IList)))))));
}; 

