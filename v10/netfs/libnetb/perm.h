/*
 * internal interfaces for permission-mapping code
 */

typedef struct {
	int sid;	/* server ID */
	int cid;	/* client ID */
} Tuid;

extern Tuid *_rfuids, *_rfgids;
extern int _rfuhp, _rfghp;	/* hash primes */
extern int *_rfsu, *_rfsg, *_rfcu, *_rfcg;	/* hash tables */
