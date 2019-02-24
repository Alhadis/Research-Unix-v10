/* messages between sysmon and sysmond */

/* a mail header */
#define MAXMAIL 64

/* the message sent */
struct msg {
    int size;
    int tag;			/* see below */
    union {
	struct ld load;
	char mail[MAXMAIL];
    } u;
};

/* tag values */
#define MAIL 0
#define LOAD 1
