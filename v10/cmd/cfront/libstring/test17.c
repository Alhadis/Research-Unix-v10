#include "list.h"

extern "C" {
	int rand();
	void srand();
}

listdeclare(int)

int	bigger(int&, int&);

main()
{
	int_list	myList;
	const sz = 500;
	int	arr[sz];
	srand();
	for (int i=0; i<sz; i++) arr[i] = i;
	for (i=sz; i; ) {
		int j = rand() % i;
		myList.put(arr[j]);
		arr[j] = arr[--i];
	}
	myList.sort(bigger);
	cout << myList << "\n";
}

int
bigger(int& a, int& b)
{
	return a < b;
}

listimplement(int)
listoutimplement(int)
/*
	int	i;
	while (cin >> i)
		myList.put(i);

	const sz = 200;
	int	arr[sz];
	srand();
	for (int i=0; i<sz; i++) arr[i] = i;
	for (i=sz; i; ) {
		int j = rand() % i;
		myList.put(arr[j]);
		arr[j] = arr[--i];
	}

	for (int i=0; i<200; i++)
		myList.put(i);
*/
