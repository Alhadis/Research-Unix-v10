/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

#include	"obj_version.h"

#define	TIME_SEP	'@'

#ifdef	STD_OBJ
#define	CMD_SZ	12
#define	CMD	{'#','!','/','b','i','n','/','e','c','h','o',' '}
#define	MAC_SZ	12
#ifndef	MAC
#define	MAC	{'v','a','x','\n','\0'}
#endif

typedef struct
{
	char	hd_cmd[CMD_SZ];
	char	hd_mac[MAC_SZ];
	long	hd_str_off;
	long	hd_str_size;
	long	hd_type_size;
	long	hd_var_size;
}
	header;

typedef enum
{
	i_lib,
	i_src,
	i_type,
	i_var,
}
	obj_items;

typedef enum
{
	t_arrayof,
	t_basetype,
	t_bitfield,
	t_dimless,
	t_elaboration,
	t_enum,
	t_ftnreturning,
	t_ptrto,
	t_structof,
	t_unionof,

	t_types,
}
	obj_types;

typedef enum
{
	v_arglist,
	v_array_size,
	v_auto,
	v_block_static,
	v_call,
	v_formal,
	v_function,
	v_global,
	v_implicit_function,
	v_initialisation,
	v_static,
	v_varargs,
}
	obj_vars;

#define	obj_code(a, b)	(((int)(a) << 4) | (int)(b))

#define	obj_item(c)	(((c) >> 4) & 0xF)
#define	obj_id(c)	((c) & 0xF)
#endif	STD_OBJ
