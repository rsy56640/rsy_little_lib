#include "RB_Tree.h"
using namespace std;
using namespace RSY_TOOL::MY_RB_Tree;



class A {
public:
	std::function<bool(pair<int, int>, pair<int, int>)> foo;
};

int main()
{

	RB_Tree<int> rbt1;
	RB_Tree<string> rbt2;


	RB_TreeImpl<string> rbtImpl1{ less<string>() };
	rbtImpl1.RB_Insert("aaa");

	std::shared_ptr<int> xx(make_shared<int>(2));
	int *p = new int(7);
	xx.reset(p);




	system("pause");
	return 0;
}
