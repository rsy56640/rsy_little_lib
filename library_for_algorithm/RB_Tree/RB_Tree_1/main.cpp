#include "RB_Tree.h"
#include <iostream>

//for comparation
#include <map>

using namespace MY_RB_Tree;

using std::cout;
using std::endl;


int main()
{
	typedef int frequence;
	typedef int category;

	RB_Tree_Iterator<int, int&, int*> it;
	*it;
	++it;


	std::map<category, frequence> mmp;
	mmp.emplace(2, 3);
	mmp.end();


	system("pause");
	return 0;
}