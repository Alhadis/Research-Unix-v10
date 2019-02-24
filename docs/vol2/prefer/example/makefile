all:	hirst.t lamport.in lamport.t simple.in simple.t type.in type.acm type.apa

hirst.t:  hirst.doc hirst ../../ADM/vol2.style
	prefer -phirst -s../../ADM/vol2.style hirst.doc >hirst.t

lamport.in: lamport.doc
	sed "s/\|/\|sp\(\|s\)/g" <lamport.doc >lamport.in

lamport.t: lamport.doc ../../ADM/vol2.ref
	prefer -p../../ADM/vol2.ref -sapa lamport.doc >lamport.t

simple.in:  simple.doc
	sed "s/\|/\|sp\(\|s\)/g" <simple.doc >simple.in

simple.t:  simple.doc ../../ADM/vol2.ref ../../ADM/vol2.style
	prefer -p../../ADM/vol2.ref -s../../ADM/vol2.style simple.doc >simple.t

type.in: type.doc
	sed "s/\|/\|sp\(\|s\)/g" <type.doc >type.in

type.acm: type.doc typerefs
	prefer -ptyperefs -sacm type.doc | fgrep -v ".nr RF" >type.acm

type.apa: type.doc typerefs
	prefer -ptyperefs -sapa type.doc | fgrep -v ".nr RF" >type.apa

clean:
	rm -f *.[ih] *.t type.acm type.apa *.in

