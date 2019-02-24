/*
 *	Any mod to this enum should be matched by modification of
 *	'where_name' in 'readonly.c'.
 */
typedef enum
{
	at_outer,
	in_arguments,
	in_block,
	in_cast,
	in_enum,
	in_struct,
	in_union,
	junk,
}
	places;
