PLIB = /usr/lib/prefer

STYLES = acm apa att ieee lsa pami spectrum

all:	$(STYLES)

clean:

clobber: clean
	rm -f $(STYLES) 

install: all
	cp $(STYLES) spe $(PLIB)/styles

print:
	prican makefile general new*

acm:	general newacm
	cat general newacm >acm

apa:	general newapa
	cat general newapa >apa

att:	general newatt
	cat general newatt >att

ieee:	general newieee
	cat general newieee >ieee

lsa:	general newlsa
	cat general newlsa >lsa

pami:	general newpami
	cat general newpami > pami

spectrum:	general newspectrum
	cat general newspectrum >spectrum

