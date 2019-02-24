STD = R I B S 
ALL = R I B S H G
STDOUT = R.out I.out B.out S.out
NONSTDOUT = G.out H.out 

update:	desc.out $(NONSTDOUT) $(STDOUT)
	touch update

desc.out:	desc $(STDOUT)
	../devtab desc

B.out:	B
	../devtab -f desc $?
G.out:	G
	../devtab -f desc $?
H.out:	H
	../devtab -f desc $?
I.out:	I
	../devtab -f desc $?
R.out:	R
	../devtab -f desc $?
S.out:	S
	../devtab -f desc $?
