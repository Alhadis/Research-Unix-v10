SHELL=/bin/sh

COBJS=$(D)/dag.o \
	$(D)/decl.o \
	$(D)/enode.o \
	$(D)/error.o \
	$(D)/expr.o \
	$(D)/init.o \
	$(D)/input.o \
	$(D)/lex.o \
	$(D)/main.o \
	$(D)/output.o \
	$(D)/profio.o \
	$(D)/simp.o \
	$(D)/stmt.o \
	$(D)/string.o \
	$(D)/sym.o \
	$(D)/tree.o \
	$(D)/types.o

c-makefile:
	@echo 1>&2 run make from a target-specific directory

$(D)/rcc:	$(COBJS) $(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(COBJS) $(OBJS)

C=../c

$(COBJS):	$(C)/c.h $(C)/token.h $(C)/ops.h
$(OBJS):	$(C)/c.h $(C)/token.h $(C)/ops.h

CCOPT=$(CFLAGS) $(INCLUDES)

$(D)/lex.o:	$(C)/keywords.h

$(D)/dag.o:	$(C)/dag.c;	$(CC) -c -o $@ $(CCOPT) $(C)/dag.c
$(D)/decl.o:	$(C)/decl.c;	$(CC) -c -o $@ $(CCOPT) $(C)/decl.c
$(D)/enode.o:	$(C)/enode.c;	$(CC) -c -o $@ $(CCOPT) $(C)/enode.c
$(D)/error.o:	$(C)/error.c;	$(CC) -c -o $@ $(CCOPT) $(C)/error.c
$(D)/expr.o:	$(C)/expr.c;	$(CC) -c -o $@ $(CCOPT) $(C)/expr.c
$(D)/init.o:	$(C)/init.c;	$(CC) -c -o $@ $(CCOPT) $(C)/init.c
$(D)/input.o:	$(C)/input.c;	$(CC) -c -o $@ $(CCOPT) $(C)/input.c
$(D)/lex.o:	$(C)/lex.c;	$(CC) -c -o $@ $(CCOPT) $(C)/lex.c
$(D)/main.o:	$(C)/main.c;	$(CC) -c -o $@ $(CCOPT) $(C)/main.c
$(D)/output.o:	$(C)/output.c;	$(CC) -c -o $@ $(CCOPT) $(C)/output.c
$(D)/profio.o:	$(C)/profio.c;	$(CC) -c -o $@ $(CCOPT) $(C)/profio.c
$(D)/simp.o:	$(C)/simp.c;	$(CC) -c -o $@ $(CCOPT) $(C)/simp.c
$(D)/stmt.o:	$(C)/stmt.c;	$(CC) -c -o $@ $(CCOPT) $(C)/stmt.c
$(D)/string.o:	$(C)/string.c;	$(CC) -c -o $@ $(CCOPT) $(C)/string.c
$(D)/sym.o:	$(C)/sym.c;	$(CC) -c -o $@ $(CCOPT) $(C)/sym.c
$(D)/tree.o:	$(C)/tree.c;	$(CC) -c -o $@ $(CCOPT) $(C)/tree.c
$(D)/types.o:	$(C)/types.c;	$(CC) -c -o $@ $(CCOPT) $(C)/types.c
