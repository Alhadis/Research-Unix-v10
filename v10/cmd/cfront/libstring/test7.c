// list test program.  reverses standard input

#include "String.h"
#include "list.h"

listdeclare(String)

String_list
revrev ( String_list x )
{
	String_list	ans;
	while ( x ) {
		String	temp;
		x.getX(temp);
		String	rtemp;
		while ( temp ) {
			char	c;
			temp.getX(c);
			rtemp.unget(c);
		}
		ans.unget(rtemp);
	}
	return ans;
}

String_list
rev ( String_list x )
{
	String_list	ans;
	while ( x ) {
		String	temp;
		x.getX(temp);
		ans.unget(temp);
	}
	return ans;
}

String_list
revrec ( String_list x )
{
	if ( x ) {
		String	temp;
		x.getX(temp);
		return revrec(x).put(temp);
	} else return x;
}

main()
{
	String_list	slist;
	String	temp;
	while ( cin >> temp)
		slist.put(temp);
	cout << "rev(slist):"  << rev(slist) << "\n";
	cout << "revrec(slist):" << revrec(slist) << "\n";
	cout << "revrev(slist):" << revrev(slist) << "\n";
}
