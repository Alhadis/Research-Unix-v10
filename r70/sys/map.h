/*
 * resource maps
 * see rmap.c
 */

struct map
{
	int	m_size;		/* size of this segment of the map */
	int	m_addr;		/* resource-space addr of start of segment */
};

/*
 * overloaded for first entry
 */

#define	m_end	m_size		/* number of struct maps in this map */
