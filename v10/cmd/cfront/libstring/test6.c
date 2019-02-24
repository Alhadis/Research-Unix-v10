// list test program.  exercise subscripts. test ==

#include "String.h"
#include "list.h"

listdeclare(String)
listdeclare(int)
listimplement(int)
listoutimplement(int)

main()
{
	String_list	slist;
	int_list	ilist;
	slist.put("zero").put("one").put("two").put("three").put("four");
	ilist.put((int)0).put(1).put(2).put(3).put(4);
	cout << slist << "\n" << ilist << "\n";
	String_list	slist1 = slist;
	int_list	ilist1;
	ilist1 = ilist;
	cout << slist[2] << " should be \"two\"\n";
	slist[2] = "not two";
	cout << slist[2] << " should not be \"two\"\n";
	cout << slist1[2] << " should be \"two\"\n";
	ilist1[2] = -2;
	cout << int(ilist[2]) << " should be 2\n";
	cout << int(ilist1[2]) << " should be -2\n";
	cout << (slist == slist1) << " should be 0\n";
	cout << (ilist == ilist1) << " should be 0\n";
	slist[2] = "two";
	ilist[2] = -2;
	cout << (slist == slist1) << " should be 1\n";
	cout << (ilist == ilist1) << " should be 1\n";
}
