#include "RB_Tree.h"
using namespace std;
using namespace RSY_TOOL::MY_RB_Tree;



class A {};

int main()
{

	RB_Tree<int> rbt1;
	RB_Tree<string> rbt2;


	RB_TreeImpl<string> rbtImpl(less<string>());

	RB_TreeImpl<A> rbtImpl(less<A>());			//what the fuck ???



	//A a1, a2;
	//bool b = a1 < a2;
	//foo(a1, a2);



	system("pause");
	return 0;
}
