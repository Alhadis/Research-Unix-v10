CFLAGS=-O
tbl: t0.o t1.o t2.o t3.o t4.o t5.o t6.o t7.o t8.o t9.o tb.o tc.o te.o tf.o tg.o ti.o tm.o tr.o ts.o tt.o tu.o tv.o
	cc -i -s -O t?.o -o tbl
t0.o t1.o t2.o t3.o t4.o t5.o t6.o t7.o t8.o t9.o tb.o tc.o te.o tf.o tg.o ti.o tm.o tr.o ts.o tt.o tu.o tv.o: t..c
install: tbl
	cp tbl /usr/bin/tbl
	rm tbl
clean:
	rm -f tbl *.o *junk
tbl-test:
	sh compr tmp/sample01
	sh compr tmp/sample02
	sh compr tmp/sample03
	sh compr tmp/sample04
	sh compr tmp/sample05
	sh compr tmp/sample06
	sh compr tmp/sample07
	sh compr tmp/sample08
	sh compeq tmp/sample09
	sh compeq tmp/sample10
	sh compeq tmp/sample11
	sh compr tmp/sample12
	sh compr tmp/sample13
	sh compr tmp/sample14
	sh compr tmp/sample15
	sh compr tmp/sample16
	sh compr tmp/sample17
	sh compr tmp/sample18
	sh compr tmp/sample19
	sh compr tmp/sample20
	sh compr tmp/sample21
	sh compr tmp/sample22
	sh compr tmp/sample23
	sh compms tmp/sample24
	sh compr tmp/sample25
	sh compr tmp/sample26
	sh compr tmp/sample27
	sh compr tmp/sample28
	sh compr tmp/sample29
	sh compr tmp/sample30
	sh compr tmp/sample31
	sh compr tmp/sample32
	sh compr tmp/sample33
	sh compms tmp/sample34
	sh compeq tmp/sample35
	sh compeq tmp/sample36
	sh compnr tmp/sample37
	sh compr tmp/sample38
	sh compr tmp/sample39
	sh compr tmp/sample40
	sh compr tmp/sample41
	sh compr tmp/sample42
	sh compeq tmp/sample43
	sh compr tmp/sample44
	sh compms tmp/sample45
	sh compms tmp/sample46
	sh compr tmp/sample47
	sh compnr tmp/sample48
	sh compr tmp/sample49
	sh compr tmp/sample50
	sh compr tmp/sample51
	sh compr tmp/sample52
	sh compr tmp/sample53
	sh compr tmp/sample54
	sh compr tmp/sample55
	sh compr tmp/sample56
tbl-ichk: /usr/bin/tbl
	/usr/bin/tbl sample1 >junk1
	cmp -s junk1 out1
tbl-comp: tbl
	cmp -s tbl /usr/bin/tbl
