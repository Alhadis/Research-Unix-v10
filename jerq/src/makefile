all:
	echo type make foo.m to install foo in /usr/jerq/mbin
%.m:	%.c
	3cc -o /usr/jerq/mbin/$%.m $%.c
	3strip -r /usr/jerq/mbin/$%.m
	-chmod a=r,ug+w /usr/jerq/mbin/$%.m
install:	56.m crabs.m hp.m icon.m jps.m m33.m pen.m ruler.m scope.m windows.m
