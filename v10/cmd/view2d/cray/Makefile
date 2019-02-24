G=1
PRECIOUS = Makefile fm lib.update eig.f EOR mvefil.f

movie:
	cray -s$(G) -o/usr/ehg/mov.j fm + eig.f
lib:
	cat lib.update EOR mvefil.f | swift cray -s$(G)

backup: Backup
Backup: $(PRECIOUS)
	push -v grigg $? `pwd`
	touch Backup
