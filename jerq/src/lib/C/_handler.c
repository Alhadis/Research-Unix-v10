
typedef void (*PFVV)();

extern PFVV _new_handler = 0;

extern PFVV set_new_handler(PFVV handler)
{
	PFVV rr = _new_handler;
	_new_handler = handler;
	return rr;
}
