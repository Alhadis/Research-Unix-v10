#define	YES		1		/* value for true                    */
#define NO		0		/* value for false                   */
#define MAXC		512		/* max # chars in filename           */
#define	MAXFILES	20		/* max # files open simultaneously   */
#define BUFSIZE		100		/* max # chars in read buffer        */
#define MAXPATH		512		/*max # chars in file specification  */

#define INSERT		"insert"	/* for |insert & |begin(insert) search */
#define BEGIN		"begin"		/* for |begin search                 */
#define END		"end"		/* for |end search                   */



struct	delims {
	int	left;
	int	right;
};

struct delims delimiters[] = {
	{ '(', ')' },
	{ '[', ']' },
	{ '<', '>' },
	{ '{', '}' },
	{ '\"','\"'},
	{ '\'','\''},
	{ '\`','\''}
};

struct file_info {
	char	*name;
	int	line_count;
};

