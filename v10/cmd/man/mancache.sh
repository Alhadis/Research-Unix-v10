cd /usr/spool/man
mkdir man1 man2 man3 man4 man5 man6 man7 man8 man9 mana manw
cd /src/man
for i in man[1-9aw]
do	for j in $i/*.?
	do	newer $j /usr/spool/man/$j &&
		nroff -man $j |
		sed '
			:x
			/^$/{
				N
				s/\n$//
				tx
			}
		' >/usr/spool/man/$j
	done
done
exec /usr/lib/manlinks
