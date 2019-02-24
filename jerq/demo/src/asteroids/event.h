typedef struct Event{
	struct Event	*next;	/* pointer */
	short		time;	/* time between activations */
	short		type;	/* usually, which object has event */
	short		dt;	/* time left before activation */
	int		(*fn)();	/* action routine */
} Event;
Event	*newevent();
/*
 * Event types
 */
#define	EKEYBOARD	0
#define	EMOUSE		0x8000
#define	EPAUSE		0x4000
#define	ESCORE		0x2000
#define	EEXPL		0x0020	/* allows 16 max explosions at once */
#define	EBOMB		0x0040	/* up to 16 bombs */
