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
	try {

		cout << "size: " << sl.size() << endl;
		sl.insert(2, 1);
		sl.insert_or_assign(2, 3);
		sl.insert_or_assign(3, 777);
		cout << "size: " << sl.size() << endl;
		cout << sl[2] << endl;
		cout << sl[3] << endl;
		cout << sl.erase(2) << endl;
		cout << sl.erase(2) << endl;
		sl.insert(5, 6);
		sl[5] = 7;
		cout << sl[5] << endl;
		sl[5] = sl[3];
		cout << sl[5] << endl;
		cout << "size: " << sl.size() << endl;
	}
	catch (std::out_of_range&)
	{
		cout << "std::out_of_range" << endl;
	}

}


int main()
{
	test();
	system("pause");
	return 0;
}