(cd libcbt; make CFLAGS=-DTEST all)
(cd cbt; make LCBT=../libcbt/libcbt.a 'CFLAGS=-DTEST -I../libcbt' all)
