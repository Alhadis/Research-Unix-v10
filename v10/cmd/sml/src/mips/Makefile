NOTANGLE=notangle
NOWEAVE=noweave
AWK=nawk
CFLAGS=-g
CC=cc

.SUFFIXES: .nw .sml .sig .awk .tex .dvi .s .print 

.nw.sml:
	$(NOTANGLE) $*.nw > $*.sml

.nw.sig:
	$(NOTANGLE) -Rsignature $*.nw > $*.sig

.nw.s:
	$(NOTANGLE) $*.nw > $*.s

.nw.awk:
	$(NOTANGLE) $*.nw > $*.awk

.nw.tex:
	$(NOWEAVE) $*.nw > $*.tex

.tex.dvi:
	tex $*.tex

.nw.dvi:
	$(NOWEAVE) $*.nw > $*.tex
	tex $*.tex

.dvi.print:
	lpr -d $*.dvi

.s.o:
	as  -o $*.o $*.s

all: mipscoder.sml mipscoder.sig opcodes.sml mipsdecode.sml mipsreal.sml \
     mips.sml mipsglue.sml emitters.sml emitters.sig

opcodes.sml: opcodes.awk opcodes.nw
	$(NOTANGLE) '-Ropcodes table' opcodes.nw | $(AWK) -f opcodes.awk > opcodes.sml

mipsdecode.sml: opcodes.awk opcodes.nw
	$(NOTANGLE) '-Ropcodes table' opcodes.nw | $(AWK) -f opcodes.awk > opcodes.sml


clean:
	rm -f *~ *.o *.log *.dvi *.tex *.makelog


