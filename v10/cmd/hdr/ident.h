struct ident
{
	ident	*id_next;
	ident	**id_last;
	char	*id_name;
	xnode	*id_type;
	token	*id_token;
	data	id_value;
	ident	*id_parent;
	ident	*id_levelp;
	object	*id_object;
	long	id_flags;
	ident	*id_memblist;
	int	id_levnum;
	long	id_index;
};

#define	ID_NAMES	(IDIS_NAME | IDIS_LABEL | IDIS_MOENUM | IDIS_TYPENAME)
#define	ID_FACTOR	(IDIS_NAME | IDIS_MOENUM)
#define	ID_TAG		(IDIS_STRUCTNAME | IDIS_UNIONNAME | IDIS_ENUMNAME)
#define	ID_MEMBER	(IDIS_MOAGG)

#define	IDIS_NAME		0x000001L
#define	IDIS_STRUCTNAME		0x000002L
#define	IDIS_UNIONNAME		0x000004L
#define	IDIS_ENUMNAME		0x000008L
#define	IDIS_FAKE		0x000010L
#define	IDIS_LABEL		0x000020L
#define	IDIS_MOAGG		0x000040L
#define	IDIS_MOENUM		0x000080L
#define	IDIS_TYPENAME		0x000100L

#define	IDIS_ARGUMENT		0x002000L

#define	IDIS_INIT		0x004000L	/* function: had the body */
#define	IDIS_ALLOC		0x008000L	/* optimiser generated */
#define	IDIS_UNDEF		0x010000L	/*
						 *	undefined aggragate or
						 *	identifier that has been
						 *	diagnosed as undefined
						 */

#define	IDIS_UANDED		0x100000L	/* someone used '&' on it */
						/* if function --- */
#define	IDIS_INTRIN		0x200000L	/* is an instrinic */
#define	IDIS_NOTINTRIN		0x400000L	/* not an intrinsic */
