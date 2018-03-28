#include "SegmentTree.h"
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;


int foo(int a, int b)
{
	return a + b;
}

//∂‘≈ƒ≤‚ ‘
class A {
public:
	A(const vector<int>& v) :value(v) {}



private:
	vector<int> value;
};


int main()
{

	srand(time(NULL));
	const int size = 50;
	vector<int> v(size);
	for (int i = 0; i < size; ++i)v[i] = 1;
	//rand() % 100;
	vector<std::pair<int, int> > errnum;

	try
	{
		SegmentTree<int> ST(v, foo, 0);
		ST.modify_augment(0, 11, 1);
		for (int i = 0; i < size; ++i)
			for (int j = 0; j <= i; ++j)
			{
				int ans = ST.query(j, i);
				int correct_ans = i - j + 1;
				if (i <= 11)correct_ans *= 2;
				else if (j <= 11)correct_ans += 12 - j;
				if (ans != correct_ans)
				{
					cout << j << "\t" << i << "\t" << ans << endl;
					errnum.push_back(std::make_pair(j, i));
				}
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