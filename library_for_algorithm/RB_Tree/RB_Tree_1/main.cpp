#include "RB_Tree.h"
#include <map>

using namespace MY_RB_Tree;

using std::cout;
using std::endl;

class A { public: int x = 1; };
class B :public A { public: int y = 4; };

int main()
{
	typedef int frequence;
	typedef int category;

	RB_Tree_Iterator<int, int&, int*> it;
	*it;
	++it;

	//std::map<category, frequence> mmp;
	//mmp.emplace(2, 3);
	A *a = new A();
	//a->y;
	auto p = static_cast<B*>(a);
	cout << p->y << endl;


	system("pause");
	return 0;
}