#include <iostream>
#include "SkipList.hpp"
#include <vector>
using namespace std;
using namespace RSY_TOOL::SkipList;


struct A {};

void test()
{
	SkipList<string, A> sl([](const string& lhs, const string& rhs) { return lhs < rhs; });


}




int main()
{
	test();
	system("pause");
	return 0;
}