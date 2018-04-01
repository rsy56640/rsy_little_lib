#include "SegmentTree.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include<Windows.h>
using namespace std;
using namespace RSY_TOOL::MySegmentTree;


//class for test
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

struct test_info
{
	vector<int>	query_left;
	vector<int> query_right;
	int modify_left;
	int modify_right;
	int value_mod;
	int aug_left;
	int aug_right;
	int value_aug;
};

void test()
{
	const int times = 10;
	static int _errno__ = 0;
	const int _size = 120;
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

			SegmentTree<int> ST(v, [](int a, int b)->int {return a + b; }, 0);

			for (int i = 0; i < _size; ++i)
			{

				int left = rand() % _size;
				int right = rand() % _size;
				int value = rand() % (2 * _size);
				if (left > right)swap(left, right);


				a.modify(left, right, value);
				ST.modify(left, right, value);
				cout << "modify: [" << left << ", " << right << "] as " << value << endl;


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


				ST.augment(left, right, augment);
				a.augment(left, right, augment);
				cout << "augment [" << left << ", " << right << "]" << " by " << augment << endl;


			}//end for-loop tests

		}//try
		catch (SegmentTreeException<int>& e)
		{
			cout << e << endl;
		}

		cout << "errno: " << _errno__ << endl;

	}


	cout << "\n\n\n";

	const int total = (_size + 1) * _size / 2;
	cout << "error num:  " << _errno__ << endl;
	cout << "total test: " << (total * times) << endl;
	cout << "error rate: " << _errno__ * 1.0 / (1.0 * total * times) << endl;
	cout << "\n\n" << endl;

}


int main()
{


	test();



	const int times = 1;
	const int _size = 1000000;
	const int test_times = 100;
	vector<double> time1(times);
	vector<double> time2(times);
	vector<vector<int> > value(times);
	vector<vector<test_info> > test(times);
	const int query_test_time = 100;

	//prepare for test_value
	for (int k = 0; k < times; ++k)
	{
		srand(static_cast<unsigned int>(time(NULL)));
		test[k].resize(test_times);
		value[k].resize(_size);
		for (int i = 0; i < _size; ++i)
			value[k][i] = rand() % (2 * _size);
		for (int i = 0; i < test_times; ++i)
		{
			int left;
			int right;
			test[k][i].query_left.resize(query_test_time);
			test[k][i].query_right.resize(query_test_time);
			for (int j = 0; j < query_test_time; ++j)
			{
				left = rand() % _size;
				right = rand() % _size;
				if (left > right)swap(left, right);
				test[k][i].query_left[j] = left;
				test[k][i].query_right[j] = right;
			}

			left = rand() % _size;
			right = rand() % _size;
			if (left > right)swap(left, right);
			test[k][i].modify_left = left;
			test[k][i].modify_right = right;
			test[k][i].value_mod = rand() % _size;

			left = rand() % _size;
			right = rand() % _size;
			if (left > right)swap(left, right);
			test[k][i].aug_left = left;
			test[k][i].aug_right = right;
			test[k][i].value_aug = rand() % _size;
		}
	}


	//test
	for (int k = 0; k < times; ++k)
	{


		DWORD start_time1 = GetTickCount();
		A a{ value[k] };
		for (int i = 0; i < test_times; ++i)
		{
			a.modify(test[k][i].modify_left, test[k][i].modify_right, test[k][i].value_mod);
			for (int j = 0; j < query_test_time; ++j)
				int temp = a.query(test[k][i].query_left[j], test[k][i].query_right[j]);
			a.augment(test[k][i].aug_left, test[k][i].aug_right, test[k][i].value_aug);
			cout << i << endl;
		}
		DWORD end_time1 = GetTickCount();
		time1[k] = end_time1 - start_time1;
		cout << 1 << "\t" << k << endl;


		DWORD start_time2 = GetTickCount();
		try
		{
			SegmentTree<int> ST(value[k], [](int a, int b)->int {return a + b; }, 0);
			for (int i = 0; i < test_times; ++i)
			{
				ST.modify(test[k][i].modify_left, test[k][i].modify_right, test[k][i].value_mod);
				for (int j = 0; j < query_test_time; ++j)
					int temp = ST.query(test[k][i].query_left[j], test[k][i].query_right[j]);
				ST.augment(test[k][i].aug_left, test[k][i].aug_right, test[k][i].value_aug);
				cout << i << endl;
			}
		}
		catch (SegmentTreeException<int>& e)
		{
			cout << e << endl;
		}
		DWORD end_time2 = GetTickCount();
		time2[k] = end_time2 - start_time2;
		cout << 2 << "\t" << k << endl;


	}


	DWORD total_time1 = 0, total_time2 = 0;
	for (int i = 0; i < times; ++i)
	{
		total_time1 += time1[i];
		total_time2 += time2[i];
		cout << time1[i] << "\t" << time2[i] << "\t" << time1[i] / time2[i] << endl;
	}

	cout << total_time1 / total_time2 << endl;

	system("pause");
	return 0;
}