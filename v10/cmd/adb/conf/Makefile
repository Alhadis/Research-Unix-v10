all: vax 11v 68v null
vax:
	mkconf adb vax v8sym v8proc >../vax/Makefile
seq:
	mkconf adb seq ptrace nullio >../seq/Makefile
11v:
	mkconf adb 11v nullsys nullexec nullinit >../11v/Makefile
68v:
	mkconf adb 68v nullcore nullframe nullsys nullexec nullinit >../68v/Makefile
null:
	mkconf adb nullfile nullcore nullframe nullsys nullio nullexec nullinit nullins nullsym >../null/Makefile

cray:
	mkconf adb nullsys nullinit cray >../cray/Makefile
g:
	(echo CC=2cc;mkconf adb g nullsys nullexec nullinit)>../g/Makefile
