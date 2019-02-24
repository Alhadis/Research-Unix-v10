all:	typerefs.i simple.t hirst.t lamport.t type.in

typerefs.i: typerefs
	pinvert typerefs

hirst.i: hirst
	pinvert hirst

simple.t:  simple.doc
	sed "s/\|/\|sp\(\|s\)/g" <simple.doc >simple.in
	prefer -p../prefer.out simple.doc >simple.t

lamport.t: lamport.doc
	sed "s/\|/\|sp\(\|s\)/g" <lamport.doc >lamport.in
	prefer -p../prefer.out -sapa lamport.doc >lamport.t

hirst.t:  hirst.i
	prefer -phirst hirst.doc >hirst.t

type.in: typerefs.i type.doc
	sed "s/\|/\|sp\(\|s\)/g" <type.doc >type.in
	prefer -ptyperefs -sacm type.doc | fgrep -v ".nr RF" >type.acm
	prefer -ptyperefs -sapa type.doc | fgrep -v ".nr RF" >type.apa

clean:
	rm -f *.[ih] *.t type.acm type.apa *.in

