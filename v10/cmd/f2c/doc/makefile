sn.t: sn.x
	eqn sn.x | troff -ms >sn.t

both: cstr.t appB.t

cstr.t: cstr.x
	tbl cstr.x | eqn | troff -ms >cstr.t

appB.t: appB.x
	troff -man appB.x >appB.t

clean:
	rm -f cstr.t appB.t
