#include <iostream>
#include "BTree.h"
#include "other_B_Tree.h"
using namespace std;
using namespace RSY_TOOL::BTree;


int main()
{

	other_B_Tree::_main();


	B_TreeImpl<int, int> b(2, less<int>());
	//b.B_Tree_find()
	b.erase(1);
	b.insert(2, 3, _INSERT_NOASSIGNMENT);


	BTree<int, int> b2;


	system("pause");
	return 0;
}