cd /src/man
for i in man[1-9a]
do man0/makelinks /usr/spool/man/$i /src/man/$i
done
