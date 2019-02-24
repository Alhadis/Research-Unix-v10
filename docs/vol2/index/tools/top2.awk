/^\.\.\.\\\"refer begin/	{ exit }
/^\.[ \t]*\\"/	{next}
/^References$/	||
/REFERENCES/	||
/\\.*\{.*References\}/	||
/\\.*\{References\}/	||
/^[0-9. ]*References$/	||
/\\end\{document\}/	||
/^\...[ ]*References[.]*$/	{ if(!indef)exit }
/^\.(de|ig|am)/	{ indef=1
	next
	}
/^\.\.([ \\]|$)/	{indef=0
	next
	}
/.*/	{ if(!indef)print $0 }
