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


const int times = 1;
static int _errno__ = 0;
const int _size = 100;

int main()
{
	for (int k = 0; k < times; ++k)
	{


		srand(static_cast<unsigned int>(time(NULL)));

		vector<int> v(_size);
		for (int i = 0; i < _size; ++i)v[i] = rand() % (2 * _size);

		[[maybe_unused]]
		std::vector<std::pair<int, int> > errnum;	//record the error

		A a{ v };

		try
		{

			SegmentTree<int> ST(v, foo, 0);

			for (int i = 0; i < _size; ++i)
			{

				int left = rand() % _size;
				int right = rand() % _size;
				int value = rand() % (2 * _size);
				if (left > right)swap(left, right);

				///*
				a.modify(left, right, value);
				ST.modify(left, right, value);
				cout << "modify: [" << left << ", " << right << "] as " << value << endl;
				//*/

				for (int j = 0; j <= i; ++j)
				{

					int ans = ST.query(j, i);
					int _ans = a.query(j, i);
					//cout << j << "\t" << i;

					if (ans != _ans)
					{
						cout << j << "\t" << i << "\t" << ans << "\t" << _ans
							<< "\t" << ans - _ans << "\t" << "\t" << ++_errno__ << endl;
						errnum.push_back(std::make_pair(j, i));
					}

					//else cout << endl;
				}

				left = rand() % _size;
				right = rand() % _size;
				int augment = rand() % 30;
				if (left > right)swap(left, right);

				///*
				ST.augment(left, right, augment);
				a.augment(left, right, augment);
				cout << "augment [" << left << ", " << right << "]" << " by " << augment << endl;
				//*/

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

		cout << "errno: " << _errno__ << endl;

	}


	cout << "\n\n\n";

	const int total = (_size - 1) * _size / 2;
	cout << _errno__ << endl;
	cout << (total * times) << endl;
	cout << _errno__ * 1.0 / (1.0 * total * times) << endl;


	system("pause");
	return 0;
}