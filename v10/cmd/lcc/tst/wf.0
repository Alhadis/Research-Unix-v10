/* wf - print word frequencies */

int count[2000];	/* frequency count */
int left[2000];		/* left subtrees */
int right[2000];	/* right subtrees */
char *words[2000];	/* words */
int next;		/* index of next free entry in count, left, etc. */

main()
{
	int root, lookup(), getword();
	char word[20];

	root = 0;
	next = 1;
	while (getword(word))
		++count[lookup(word, &root)];
	tprint(root);
	return 0;
}

/* err - print error message s and die	*/
err(s)
char *s;
{
	printf("? %s\n", s);
	exit(1);
}

/* getword - get next input word into buf, return 0 on EOF */
int getword(buf)
char *buf;
{
	char *s;
	int c;

	while ((c = getchar()) != -1 && isletter(c) == 0)
		;
	for (s = buf; c = isletter(c); c = getchar())
		*s++ = c;
	*s = 0;
	if (s > buf)
		return 1;
	return 0;
}

/* isletter - return folded version of c if it is a letter, 0 otherwise */
int isletter(c)
int c;
{
	if (c >= 'A' && c <= 'Z')
		c += 'a' - 'A';
	if (c >= 'a' && c <= 'z')
		return c;
	return 0;
}

/* lookup - lookup word in tree; install if necessary */
int lookup(word, p)
char *word;
int *p;
{
	int cond;
	char *malloc();

	if (*p) {
		cond = strcmp(word, words[*p]);
		if (cond < 0)
			return lookup(word, &left[*p]);
		else if (cond > 0)
			return lookup(word, &right[*p]);
		else
			return *p;
	}
	if (next >= 2000)
		err("out of node storage");
	count[next] = left[next] = right[next] = 0;	
	words[next] = malloc(strlen(word) + 1);
	if (words[next] == 0)
		err("out of word storage");
	strcpy(words[next], word);
	return *p = next++;
}

/* tprint - print tree */
tprint(tree)
int tree;
{
	if (tree) {
		tprint(left[tree]);
		printf("%d\t%s\n", count[tree], words[tree]);
		tprint(right[tree]);
	}
}

/* strcmp - compare s1 and s2, return <0, 0, or >0 */
int strcmp(s1, s2)
char *s1, *s2;
{
	while (*s1 == *s2) {
		if (*s1++ == 0)
			return 0;
		++s2;
	}
	if (*s1 == 0)
		return -1;
	else if (*s2 == 0)
		return 1;
	return *s1 - *s2;
}
