TBL=tbl
TROFF=/usr/bin/troff
VTROFF=/usr/ucb/vtroff

vpr:
	$(TBL) mail?.nr | $(VTROFF) -me

lpr:
	$(TBL) mail?.nr | nroff -me -Tlpr | lpr

troff:
	$(TBL) mail?.nr | troff -me

vcopy:
	$(TBL) mail?.nr | $(VTROFF) -me -t > vout
