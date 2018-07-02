#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include "smartPointer.h"
using namespace std;

void foo()
{

}

class A {
public:
	A(int x)
		:i(x)
	{
		cout << "ctor " << i << endl;
	}
	void foo()
	{
		cout << "foo() " << i << endl;
	}
	~A() {
		cout << "dtor " << i << endl;
	}
	int i;
};
class B :public A
{};

void del(A*) {}

int main()
{
	std::function<void(A*)> d = del;

	//shared_ptr<A>  pa(new A{1}, d);
	//shared_ptr<A>  pb = _STD make_shared<A>(1);


	//A* p = pa.get();


	using namespace RSY_TOOL::Smart_Pointer;
	SmartPtr<int> si;
	SmartPtr<int> s2(new int(2));
	SmartPtr<A> pa1(new A(1));
	SmartPtr<A> pa2 = pa1;
	SmartPtr<A> pa3(new A(3));
	SmartPtr<A> pa4(new A(4));
	SmartPtr<A> pa5(new A(5));
	pa2 = std::move(pa3);
	pa1 = std::move(pa2);
	pa1->foo();
	SmartPtr<A> pa6(std::move(pa3));
	pa3 = std::move(pa1);
	pa5 = pa3;
	pa2 = pa4;
	//pa1->foo();
	pa4->foo();

	system("pause");
	return 0;
}
