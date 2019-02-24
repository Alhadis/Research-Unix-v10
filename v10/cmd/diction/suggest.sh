trap 'rm -f $$; exit' 1 2 3 15
D=/usr/lib/style/suggest.d
if test $1
then
cat >$$ <<dn
/ $*.*	/s/\(.*\)	\(.*\)/use "\2" for "\1"/p
dn
sed -n -f $$ $D; rm $$
fi
while echo "phrase?"; read x
do
cat >$$ <<dn
/ $x.*	/s/\(.*\)	\(.*\)/use "\2" for "\1"/p
dn
case $x in
and/or)
	sed "s/and./and./" $$ >a$$
	sed -n -f a$$ $D;rm $$ a$$;;
q)
	rm $$; exit;;
[a-z]*)
sed -n -f $$ $D; rm $$;;
*) rm $$;;
esac
done
