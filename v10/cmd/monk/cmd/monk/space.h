char	*global_internal_defs[] = {
	MONK_TEXT, MONK_FILE, MONK_SAVE, MONK_RESTORE, U_NL_NL, U_NL_WHITE, NULL
};

char	*doc_internal_defs[] = {
	DOC_TEXT, DOC_SAVE, DOC_RESTORE, NULL
};

struct delim_pairs {
	int	open;
	int	close;
};

struct	delim_pairs	delim_pairs[] = {
	{ '(', ')' },
	{ '[', ']' },
	{ '{', '}' },
	{ '<', '>' },
	{ '\"', '\"' },
	{ '\'', '\'' },
	{ '\`', '\'' },
	{ '\0', '\0' }
};
