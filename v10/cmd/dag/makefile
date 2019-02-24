CC = nCC
CFLAGS = -O -DV9 -DMICC

INTEROBJS = parsedag.h parsedag.c
CPIO_SRC = parsedag.y dag.c draw_dag.c dag_start.c dag_levels.c dag_misc.c \
	dag_pos.c dag_ranks.c dag_edge.c dag_simplex.c dag_spline.c dag_stem.c \
	emit_pic.c emit_ps.c emit_gd.c node_t.c edge_t.c util.c \
	trie.c kpvmalloc.c TrieFA.ins.c TrieFA.h dag.h draw_dag.h\
	defaults.h paths.h daglib.pic daglib.ps trie_input\
	cfdag.awk dagpsjobs gprof2dag.awk dag.1 dag.doc Makefile makefile README

OBJS = parsedag.o dag.o draw_dag.o dag_start.o dag_levels.o dag_misc.o \
	dag_pos.o dag_ranks.o dag_edge.o dag_simplex.o dag_spline.o dag_stem.o \
	emit_pic.o emit_ps.o emit_gd.o node_t.o edge_t.o util.o\
	trie.o kpvmalloc.o

dag : $(OBJS)
	$(CC) $(CFLAGS) -o dag $(OBJS) -lm

parsedag.h : parsedag.y
	yacc -d parsedag.y
	mv y.tab.c parsedag.c
	mv y.tab.h parsedag.h

parsedag.c : parsedag.h

kpvmalloc.o : kpvmalloc.c
	cc $(CFLAGS) -c kpvmalloc.c

#Include file dependencies

parsedag.o dag.o emit_gd.o emit_pic.o emit_ps.o trie.o : dag.h draw_dag.h
dag.o : TrieFA.ins.c
dag.o trie.o : TrieFA.h 
dag.o emit_gd.o emit_pic.o emit_ps.o : parsedag.y
emit_pic.o emit_ps.o util.o : paths.h
edge_t.c node_t.c util.c : defaults.h
dag_edge.o dag_levels.o dag_misc.o dag_pos.o dag_ranks.o dag_simplex.o \
 dag_spline.o dag_stem.o dag_start.o draw_dag.o : draw_dag.h


clean :
	rm -f $(OBJS)

clobber :
	rm -f $(OBJS) $(INTEROBJS) dag

cpio :
	ls $(CPIO_SRC) examples/* | cpio -ocv > dag.cpio
	
