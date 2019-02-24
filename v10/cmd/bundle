PATH=/bin:/usr/bin
echo '# To unbundle, sh this file'
for i
do
	echo "echo $i 1>&2"
	echo "sed 's/.//' >$i <<'//GO.SYSIN DD $i'"
	sed 's/^/-/' $i
	echo "//GO.SYSIN DD $i"
done
