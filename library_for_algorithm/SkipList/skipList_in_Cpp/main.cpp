#include <iostream>
#include "SkipList.hpp"
using namespace std;
using namespace RSY_TOOL::SkipList;

void test()
{

}


struct A {
	template <
		typename U,
		std::enable_if_t<std::is_convertible<U, A>::value> * = nullptr>
		A(U &&u) {}
};


int main()
{
	test();
	int x, y = 0;



	system("pause");
	return 0;
}