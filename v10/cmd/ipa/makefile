ipa_trans:	ipa_trans.lex
	lex ipa_trans.lex
	cc -o ipa_trans lex.yy.c -ll
install:
	strip ipa_trans
	cp ipa_trans /usr/lib
	cp ipa /usr/bin
clean:
	rm -f lex.yy.c ipa_trans
