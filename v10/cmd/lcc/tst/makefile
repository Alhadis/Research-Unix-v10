tst-makefile:
	@echo 1>&2 run make from a target-specific directory

test:	$(V)/tst/8q.s \
	$(V)/tst/array.s \
	$(V)/tst/cf.s \
	$(V)/tst/cq.s \
	$(V)/tst/cvt.s \
	$(V)/tst/fields.s \
	$(V)/tst/front.s \
	$(V)/tst/incr.s \
	$(V)/tst/init.s \
	$(V)/tst/limits.s \
	$(V)/tst/paranoia.s \
	$(V)/tst/sort.s \
	$(V)/tst/spill.s \
	$(V)/tst/stdarg.s \
	$(V)/tst/struct.s \
	$(V)/tst/switch.s \
	$(V)/tst/wf1.s \
	$(V)/tst/yacc.s

T=../tst
CMD=@cd $(V); ../run $(TARGET) $(OS)

$(V)/tst/8q.s:		$(RUN) $(D)/rcc $(T)/8q.c       $(T)/8q.0;	$(CMD) 8q
$(V)/tst/array.s:	$(RUN) $(D)/rcc $(T)/array.c    $(T)/array.0;	$(CMD) array
$(V)/tst/cf.s:		$(RUN) $(D)/rcc $(T)/cf.c       $(T)/cf.0;	$(CMD) cf
$(V)/tst/cq.s:		$(RUN) $(D)/rcc $(T)/cq.c       $(T)/cq.0;	$(CMD) cq
$(V)/tst/cvt.s:		$(RUN) $(D)/rcc $(T)/cvt.c      $(T)/cvt.0;	$(CMD) cvt
$(V)/tst/fields.s:	$(RUN) $(D)/rcc $(T)/fields.c   $(T)/fields.0;	$(CMD) fields
$(V)/tst/front.s:	$(RUN) $(D)/rcc $(T)/front.c    $(T)/front.0;	$(CMD) front
$(V)/tst/incr.s:	$(RUN) $(D)/rcc $(T)/incr.c     $(T)/incr.0;	$(CMD) incr
$(V)/tst/init.s:	$(RUN) $(D)/rcc $(T)/init.c     $(T)/init.0;	$(CMD) init
$(V)/tst/limits.s:	$(RUN) $(D)/rcc $(T)/limits.c   $(T)/limits.0;	$(CMD) limits
$(V)/tst/paranoia.s:	$(RUN) $(D)/rcc $(T)/paranoia.c	$(T)/paranoia.0;$(CMD) paranoia
$(V)/tst/sort.s:	$(RUN) $(D)/rcc $(T)/sort.c     $(T)/sort.0;	$(CMD) sort
$(V)/tst/spill.s:	$(RUN) $(D)/rcc $(T)/spill.c    $(T)/spill.0;	$(CMD) spill
$(V)/tst/stdarg.s:	$(RUN) $(D)/rcc $(T)/stdarg.c   $(T)/stdarg.0;	$(CMD) stdarg
$(V)/tst/struct.s:	$(RUN) $(D)/rcc $(T)/struct.c   $(T)/struct.0;	$(CMD) struct
$(V)/tst/switch.s:	$(RUN) $(D)/rcc $(T)/switch.c   $(T)/switch.0;	$(CMD) switch
$(V)/tst/wf1.s:		$(RUN) $(D)/rcc $(T)/wf1.c      $(T)/wf1.0;	$(CMD) wf1
$(V)/tst/yacc.s:	$(RUN) $(D)/rcc $(T)/yacc.c     $(T)/yacc.0;	$(CMD) yacc

OPTS=HOST=$(HOST) HOSTOS=$(HOSTOS) CC='lcc -B$(HOST)-$(HOSTOS)/ -d0.1 -A' \
	CFLAGS='-I../include/$(HOST)_$(HOSTOS) $(CFLAGS)' \
	LDFLAGS='$(LDFLAGS)'

triple:	$(D)/rcc
	rm -f $(D)/*.o
	make $(OPTS) $(D)/rcc
	-od +8 <$(D)/rcc >$(D)/tst/od2
	rm -f $(D)/*.o
	make $(OPTS) $(D)/rcc
	-od +8 <$(D)/rcc >$(D)/tst/od3
	cmp $(D)/tst/od[23] && rm $(D)/tst/od[23]

clean:
	rm -f $(D)/*.o $(D)/tst/od[23] $(V)/tst/*.[12so]
	-cd $(V)/tst; rm -f 8q array cf cq cvt fields front incr init \
		limits paranoia sort spill stdarg struct switch wf1 yacc

clobber:	clean
		rm -f $(D)/rcc
