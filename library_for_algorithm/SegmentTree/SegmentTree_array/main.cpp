#include "SegmentTree.h"
#include <iostream>

using namespace std;

int foo(int a, int b)
{
	return a + b;
}


int main()
{

	vector<int> v = { 1,2,7,8,5 };

	try
	{
		SegmentTree<int> h(v, foo, 0);

		int a = h.query(0, 2);

		h.modify(0, 4);

		int b = h.query(0, 1);

		h.modify(2, [](int x)->int {return x + 4; });

		int c = h.query(2, 3);

	}
	catch (SegmentTreeException<int>& e)
	{
		cout << e << endl;
	}


	system("pause");
	return 0;
}