/* Copyright 1984,1985,1986,1987,1988,1989,1990 AT&T Bell Laboratories */

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_break(char *name, int line, char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_break();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern struct enode *cin_compile(struct enode *code);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern struct enode *cin_compile();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_dump(char *name);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_dump();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_epp(char *func);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_epp();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern struct enode *cin_eprint(struct enode *code);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern struct enode *cin_eprint();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern char	*cin_error_code_set(char *message, char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern char	*cin_error_code_set();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern struct enode *cin_eval(struct enode *code);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern struct enode *cin_eval();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern struct ident *cin_find_ident(char *name);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern struct ident *cin_find_ident();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern struct nlist *cin_find_nlist(unsigned long addr);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern struct nlist *cin_find_nlist();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_info(char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_info();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void(*cin_info_set(char *string, void(*routine)()))();
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void(*cin_info_set())();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_load(char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_load();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_log(char *name);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_log();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern char	*cin_ltof(int line);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern char	*cin_ltof();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_make(char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_make();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_pop(int level);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_pop();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_pp(char *func, int level);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_pp();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern struct enode *cin_print(struct enode *code, int level);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern struct enode *cin_print();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_quit();
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_quit();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern struct enode *cin_read(char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern struct enode *cin_read();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_reset();
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_reset();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_return();
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_return();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_run(char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_run();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern char	*cin_slashify(char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern char	*cin_slashify();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_spy(char *func, char *name, char *mod, char *ref);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_spy();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_step();
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_step();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_stepin();
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_stepin();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_stepout();
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_stepout();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_stopin(char *func, char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_stopin();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern char	*cin_sync(char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern char	*cin_sync();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern void cin_system();
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern void cin_system();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_unbreak(char *name, int line, char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_unbreak();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_unload(char *name);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_unload();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_unspy(char *func, char *name, char *mod, char *ref);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_unspy();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_unstopin(char *func, char *string);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_unstopin();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_unwrapper(char *func, void(*before)(), void(*after)());
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_unwrapper();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_view(char *name);
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_view();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

#if	defined(__STDC__) || defined(__cplusplus)
extern int	cin_wrapper(char *func, void(*before)(), void(*after)());
#else	/* defined(__STDC__) || defined(__cplusplus)*/
extern int	cin_wrapper();
#endif	/* defined(__STDC__) || defined(__cplusplus)*/

extern int	cin_argc;
extern char	**cin_argv;
extern int	cin_err_fd;
extern char	*cin_filename;
extern int	cin_in_fd;
extern int	cin_level;
extern char	*cin_libpath;
extern int	cin_lineno;
extern int	cin_out_fd;
extern char	*cin_prompt;
extern struct stackelem *cin_stack;
extern struct view *cin_views;
