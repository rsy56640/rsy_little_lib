#include <iostream>
#include "SkipList.hpp"
#include <string>
#include <vector>
using namespace std;
using namespace RSY_TOOL::SkipList;


struct A {};

void test()
{
	SkipList<int, int> sl([](const int& lhs, const int& rhs) { return lhs < rhs; });
	cout << sl.insert(2, 1) << endl;;
	cout << sl.insert_or_assign(2, 3) << endl;
	cout << sl.insert_or_assign(3, 777) << endl;
	cout << sl[2] << endl;
	cout << sl[3] << endl;
	cout << sl.erase(2) << endl;
	cout << sl.size() << endl;
}


int main()
{
	test();
	system("pause");
	return 0;
}