/*
 * mount structure
 * used to keep track of fs filesystems
 * also used, stupidly, by the vm code
 */

struct	mount {
	struct inode *m_dev;	/* special file where filesystem lives */
	struct inode *m_mroot;	/* root of this filesystem */
};
