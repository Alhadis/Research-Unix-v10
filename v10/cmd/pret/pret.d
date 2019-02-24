	/*
	** 3-dimensional table:
	**	one row per state
	**	one column per event
	**	one `pilar' per possible response
	*/

 struct ENTRY {
	int nrpils;		/* size of pilar stack */
	struct PILAR *pilar;
	struct ENTRY *nextrow;
	struct ENTRY *nextcol;
 };
 struct PILAR {
	int transf;		/* row transition function   */
	int code;		/* output message or bltin index */
	struct PILAR *nxtp;	/* last pilar points to NULL */
 };

 struct COL {
	int coltype;		/* msg, spont, tau, default, task, condition */
	int ccode;
 };

 struct ROW {
	char name[256];		/* can be composite name   */
	char reached;		/* is row reachable ?      */
	char refcount;		/* label reference index   */
	char labeled;		/* start of do-loop or label */
	int mapping;		/* indicates transit rows  */
	int maptwo;		/* renumbers reduced table */
 };
