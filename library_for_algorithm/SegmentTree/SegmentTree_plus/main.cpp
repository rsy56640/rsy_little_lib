#include "SegmentTree.h"
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
using namespace RSY_TOOL::MySegmentTree;

int foo(int a, int b)
{
	return a + b;
}

//∂‘≈ƒ≤‚ ‘
class A {
public:
	A(const vector<int>& v) :value(v) {}
	int query(int start, int end)
	{
		int sum = 0;
		for (int i = start; i <= end; ++i)
			sum += value[i];
		return sum;
	}
	void modify(int start, int end, int _value)
	{
		for (int i = start; i <= end; ++i)
			value[i] = _value;
	}
	void augment(int start, int end, int aug)
	{
		for (int i = start; i <= end; ++i)
			value[i] += aug;
	}

private:
	vector<int> value;
};




int main()
{
	cout << time(NULL) << endl;
	srand(static_cast<unsigned int>(time(NULL)));
	const int size = 16;
	vector<int> v(size);
	for (int i = 0; i < size; ++i)v[i] = rand() % 100;

	//record the error
	//[[maybe_unused]]
	std::vector<std::pair<int, int> > errnum;

	A _ST{ v };

	try
	{

		SegmentTree<int> ST(v, foo, 0);

		for (int i = 0; i < size; ++i)
		{

			ST.modify(3, 6, i + 1);
			_ST.modify(3, 6, i + 1);

			int _ans = _ST.query(0, 4);
			int ans = ST.query(0, 4);

			for (int j = 0; j <= i; ++j)
			{

				int ans = ST.query(j, i);
				int _ans = _ST.query(j, i);
				cout << j << "\t" << i;

				if (ans != _ans)
				{
					cout << "\t" << ans << "\t" << _ans << endl;
					errnum.push_back(std::make_pair(j, i));
				}

				else cout << endl;

			}

			//ST.augment(28, 44, 1);
			//_ST.augment(28, 44, 1);

		}


		/*
		int a = ST.query(0, 2);

		ST.modify(0, 4);

		int b = ST.query(0, 1);

		ST.modify(2, [](int x)->int {return x + 4; });

		int c = ST.query(2, 3);
		*/

	}
	catch (SegmentTreeException<int>& e)
	{
		cout << e << endl;
	}



	system("pause");
	return 0;
}