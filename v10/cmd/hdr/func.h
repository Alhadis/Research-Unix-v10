/*
 *	Information about the current function.
 */
typedef struct
{
    char	*fn_name;
    cnode	*fn_code;
    ident	*fn_args;
    ident	*fn_locals;
    token	*fn_token;
    token	*fn_ret_token;
    token	*fn_vret_token;
    xnode	*fn_type;
}
	func_data;

extern func_data	func;
