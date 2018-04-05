#include "RB_Tree.h"
#include <iostream>

//for comparation
#include <map>

using namespace RSY_TOOL::MY_RB_Tree;

using std::cout;
using std::endl;

template<class _Ty1, class _Ty2>
struct Comp
{
	constexpr bool
		operator()
		(const std::pair<_Ty1, _Ty2>& lhs, const std::pair<_Ty1, _Ty2>& rhs) noexcept
	{
		return true;
	}
};

int main()
{
	typedef int frequence;
	typedef int category;

	RB_Tree_Iterator<int, int&, int*> it;
	*it;
	++it;
	//RB_Tree<int, int, int, Comp<int, int>> my_rbt;

	std::map<category, frequence> mmp;
	mmp.emplace(2, 3);
	mmp.end();


	system("pause");
	return 0;
}