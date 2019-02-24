ln -s /usr/man/man[13]/cbt.[13] .
nroff -man cbt.1 >nroff.cbt.1
nroff -man cbt.3 >nroff.cbt.3
ln -s /usr/src/libcbt /usr/src/cmd/cbt .
cpio -oc >cbt.cpio <<!
README
cbt.1
cbt.3
nroff.cbt.1
nroff.cbt.3
test.sh
cbt/makefile
cbt/cbt
cbt/btadd.c
cbt/btbuild.c
cbt/btcat.c
cbt/btcreat.c
cbt/btdelete.c
cbt/btlook.c
cbt/btran.c
cbt/btreport.c
cbt/btsquash.c
cbt/verify
libcbt/makefile
libcbt/cbt.h
libcbt/pr.h
libcbt/bdelete.c
libcbt/bt.c
libcbt/bwrite.c
libcbt/diskrd.c
libcbt/diskwrt.c
libcbt/lib.c
libcbt/seek.c
libcbt/tran.c
!
rm *cbt.[13] ./libcbt ./cbt
