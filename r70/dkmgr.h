

/*
 * structure returned by dkmgr()
 * for an incoming call from datakit
 */

struct mgrmsg {
	short	m_chan ;		/* chanel number of call */
	short	m_tstamp ;		/* time stamp of msg from controller */
	short	m_traffic ;		/* traffic type requested */
	char *	m_dial ;		/* pointer to dialstring */
	char *	m_service ;		/* pointer to service name requested */
	char *	m_source ;		/* pointer to source-id of call */
	char *	m_origin ;		/* pointer to node.mod.chan */
	char *	m_uid ;			/* pointer to userid string from source */
	char *	m_param1 ;		/* pointer to first param from source */
	char *	m_param2 ;		/* pointer to second */
} ;

