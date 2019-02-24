BEGIN {print ".GS"; print "draw nodes pointsize 30;"; print "separate ranks 6;"}

{	sub("[0-9]*",""); 
	for (i = 0; substr($0,i,1) == "\t"; i++);
	i--;
	inp = substr($0,i+1,length($0));
	sub(":.*","",inp);
	if (i > 1) printf("%s %s ;\n",stack[i - 1],inp);
	stack[i] = inp;
}

END {print ".GE";}
