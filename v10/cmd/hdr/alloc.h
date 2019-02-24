extern char	*alloc_end;
extern char	*alloc_ptr;
extern cfrag	*kfree;
extern cfrag	*ktmp;
extern cnode	*cfree;
extern cnode	*ctmp;
extern token	*tfree;
extern token	*ttmp;
extern xnode	*xfree;
extern xnode	*xtmp;

extern char	*alloc_fill();
extern char	*alloc_vector();
extern char	*chunk();
extern char	*rechunk();
extern cfrag	*kreplenish();
extern cnode	*creplenish();
extern token	*treplenish();
extern void	free_chunk();
extern xnode	*xreplenish();

#define	alloc(n)	((alloc_ptr += (n)) > alloc_end ? alloc_fill(n) : (alloc_ptr - (n)))
#define	talloc(x)	(x *)alloc(sizeof(x))
#define	vector(p, n, t)	(t *)alloc_vector((char *)p, (n) * sizeof (t))

#if	!defined(ALLOC_CALLS) && !defined(ALLOC_INTRIN)
#define	new_cnode()	(cfree = (ctmp = cfree == NULL ? creplenish() : cfree)->c_next, ctmp)
#define	free_cnode(c)	(ctmp = cfree, (cfree = c)->c_next = ctmp)

#define	new_cfrag()	(kfree = (ktmp = kfree == NULL ? kreplenish() : kfree)->k_next, ktmp)
#define	free_cfrag(k)	(ktmp = kfree, (kfree = k)->k_next = ktmp)

#define	new_token()	(tfree = (ttmp = tfree == NULL ? treplenish() : tfree)->t_value.t, ttmp)
#define	free_token(p)	(ttmp = tfree, (tfree = p)->t_value.t = ttmp)

#define	new_xnode()	(xfree = (xtmp = xfree == NULL ? xreplenish() : xfree)->x_left, xtmp)
#define	free_xnode(x)	(xtmp = xfree, (xfree = x)->x_left = xtmp)
#else
extern void	free_cnode();
extern cnode	*new_cnode();
extern void	free_cfrag();
extern cfrag	*new_cfrag();
extern void	free_token();
extern token	*new_token();
extern void	free_xnode();
extern xnode	*new_xnode();
#endif
