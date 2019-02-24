typedef void* PV;
typedef void (*PF)(PV,int,int,int,int,int,int,int,int);
typedef void (*PFI)(PV,int ...);
/*
	ERROR: cannot handle classes with more than 8 virtual bases
*/

// functions that manage the map from arrays to their element counts
extern void	__insert_new_array(PV key, int count);
	// key is a pointer to a new array.  It must
	//	be non-zero
	//	not already be in the table
	// count is the number of elements in the array.  May be zero
extern int	__remove_old_array(PV key);
	// removes an old array from the table.  Returns the count or -1 if not found

#ifdef __cplusplus
extern "C"
#endif
PV __vec_new(PV op, int n, int sz, PV f)
/*
	allocate a vector of "n" elements of size "sz"
	and initialize each by a call of "f"
*/
{
	if (op == 0) {
		op = PV( new char[n*sz] );
		if (op) __insert_new_array(op, n);
	}
	if (op && f) {
		register char* p = (char*) op;
		register char* lim = p + n*sz;
		register PF fp = PF(f);
		while (p < lim) {
			(*fp) (PV(p),0,0,0,0,0,0,0,0);
			p += sz;
		}
	}
	return PV(op);
}

#ifdef __cplusplus
extern "C"
#endif
void __vec_delete(PV op, int n, int sz, PV f, int del, int)
{
	if (op) {
		if (f) {
			if (del && n == -1)
				n = __remove_old_array(op);	// -1 if not found
			register char* cp = (char*) op;
			register char* p = cp;
			register PFI fp = PFI(f);
			p += n*sz;
			while (p > cp) {
				p -= sz;
				(*fp)(PV(p), 2);  // destroy VBC, don't delete
			}
		}
		if (del) delete op;
	}
}

