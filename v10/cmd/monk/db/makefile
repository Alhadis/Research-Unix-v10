M4 = @m4

A4 = awk -f a4

# CHANGED: 2/18/91 to install on hunny
# the following was the assignment on bowell:
MHOME = /usr/lib/monk

# to install on hunny, the following needs to be used:
#MHOME = /usr/local/lib/monk
# END OF CHANGE

INSDIR = $(MHOME)/db

COMPRESS = $(MHOME)/compress -d .

FRC =

DBS = \
	acm \
	attrib \
	centerpb \
	cspress \
	form1 \
	global \
	im \
	kluwer \
	letter \
	macro \
	memo \
	model \
	research \
	rp \
	song \
	tc \
	tm

CDBS = \
	acm.comp \
	centerpb.comp \
	cspress.comp \
	form1.comp \
	global.comp \
	im.comp \
	kluwer.comp \
	letter.comp \
	memo.comp \
	model.comp \
	research.comp \
	rp.comp \
	song.comp \
	tc.comp \
	tm.comp

all: doc comp

doc: $(DBS)

comp: $(CDBS)

acm: \
	acm.m \
	global.h \
	macro.h \
	titlebox.h \
	titlebox.m \
	$(FRC)
	$(A4) acm.m > acm
	@echo "acm updated"

acm.comp: acm attrib $(FRC)
	$(COMPRESS) acm

attrib: \
	attrib.m \
	list.h \
	macro.h \
	style.h \
	$(FRC)
	$(A4) attrib.m > attrib
	@echo "attrib updated"

centerpb: \
	centerpb.m \
	macro.h \
	$(FRC)
	$(A4) centerpb.m > centerpb
	@echo "centerpb updated"

centerpb.comp: centerpb attrib $(FRC)
	$(COMPRESS) centerpb

cspress: \
	cspress.m \
	global.h \
	macro.h \
	titlebox.h \
	titlebox.m \
	$(FRC)
	$(A4) cspress.m > cspress
	@echo "cspress updated"

cspress.comp: cspress attrib $(FRC)
	$(COMPRESS) cspress

form1: \
	form1.m \
	global.h \
	macro.h \
	$(FRC)
	$(A4) form1.m > form1
	@echo "form1 updated"

form1.comp: form1 attrib $(FRC)
	$(COMPRESS) form1

global: \
	global.m \
	envir.h \
	envir.m \
	global.h \
	index.m \
	list.h \
	list.m \
	macro.h \
	preproc.h \
	preproc.m \
	section.h \
	section.m \
	set.m \
	special.m \
	style.h \
	style.m \
	t_contents.m \
	whitespace.m \
	$(FRC)
	$(A4) global.m > global
	@echo "global updated"

global.comp: attrib global macro $(FRC)
	$(COMPRESS)

im: \
	im.m \
	coversheet.h \
	coversheet.m \
	envir.h \
	macro.h \
	preproc.h \
	titlebox.h \
	titlebox.m \
	$(FRC)
	$(A4) im.m > im
	@echo "im updated"

im.comp: im attrib $(FRC)
	$(COMPRESS) im

kluwer: \
	kluwer.m \
	global.h \
	macro.h \
	section.h \
	$(FRC)
	$(A4) kluwer.m > kluwer
	@echo "kluwer updated"

kluwer.comp: kluwer attrib $(FRC)
	$(COMPRESS) kluwer

letter: \
	letter.m \
	macro.h \
	$(FRC)
	$(A4) letter.m > letter
	@echo "letter updated"

letter.comp: letter attrib $(FRC)
	$(COMPRESS) letter

macro: \
	macro.m \
	macro.h \
	style.h \
	$(FRC)
	$(A4) macro.m > macro
	@echo "macro updated"

memo: \
	memo.m \
	macro.h \
	titlebox.h \
	titlebox.m \
	$(FRC)
	$(A4) memo.m > memo
	@echo "memo updated"

memo.comp: memo attrib $(FRC)
	$(COMPRESS) memo

model: \
	model.m \
	global.h \
	macro.h \
	titlebox.h \
	titlebox.m \
	$(FRC)
	$(A4) model.m > model
	@echo "model updated"

model.comp: model attrib $(FRC)
	$(COMPRESS) model

research: \
	research.m \
	global.h \
	macro.h \
	$(FRC)
	$(A4) research.m > research
	@echo "research updated"

research.comp: research attrib $(FRC)
	$(COMPRESS) research

rp: \
	rp.m \
	macro.h \
	titlebox.h \
	titlebox.m \
	$(FRC)
	$(A4) rp.m > rp
	@echo "rp updated"

rp.comp: rp attrib $(FRC)
	$(COMPRESS) rp

song: \
	song.m \
	global.h \
	$(FRC)
	$(A4) song.m > song
	@echo "song updated"

song.comp: song attrib $(FRC)
	$(COMPRESS) song

tc: \
	tc.m \
	coversheet.h \
	coversheet.m \
	envir.h \
	macro.h \
	preproc.h \
	titlebox.h \
	titlebox.m \
	$(FRC)
	$(A4) tc.m > tc
	@echo "tc updated"

tc.comp: tc attrib $(FRC)
	$(COMPRESS) tc

tm: \
	tm.m \
	coversheet.h \
	coversheet.m \
	envir.h \
	macro.h \
	preproc.h \
	titlebox.h \
	titlebox.m \
	$(FRC)
	$(A4) tm.m > tm
	@echo "tm updated"

tm.comp: tm attrib $(FRC)
	$(COMPRESS) tm

clean:

clobber: clean
	-rm -f $(DBS) $(CDBS)

install: all
	cp $(DBS) $(CDBS) $(INSDIR)

debug: doc
	cp $(DBS) $(INSDIR)

summary: $(DBS)
	@for i in $(DBS) ; \
	do \
		echo -e "\n\n$$i doc type\n" ; \
		grep '|comment<summary: ' $$i | \
		    sed -e 's/|comment<summary: \(.*\)>/\1/' | sort -u ; \
	done > summary
	@echo -e '\n' >> summary
	@cat todo.m >> summary
	@echo summary updated
