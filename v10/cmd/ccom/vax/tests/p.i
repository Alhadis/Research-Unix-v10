typedef struct Menu{
	char	**item;
} Menu;

Menu menus[10] ; 

char m_strings[100][10];

m_init()
{
	register m, i, s ;

			menus[m].item[i] = &(m_strings[s++][0]);	/* r0 */
}
