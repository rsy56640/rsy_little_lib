#include "RB_Tree.h"
#include <set>
#include <map>
#include <functional>
using namespace std;
using namespace RSY_TOOL::MY_RB_Tree;

namespace myInteger
{

	class Integer
	{
	public:

		~Integer()
		{
			cout << "destroy " << val << endl;
		}

		Integer(int x = 0) :val(x) {}

		Integer(const Integer& other) :val(other.val) {}

		Integer(Integer&& other) :val(other.val) {}

		Integer& operator=(const Integer& other)& = default;

		Integer& operator=(Integer&& other)& = default;

		Integer& operator+=(const Integer& other)
		{
			this->val += other.val;
			return *this;
		}

		Integer& operator-=(const Integer& other)
		{
			this->val -= other.val;
			return *this;
		}

		bool operator==(const Integer& other)
		{
			return this->val == other.val;
		}

		friend ostream& operator<<(ostream& os, const Integer& other)
		{
			os << other.val;
			return os;
		}

	private:

		int val;
	};


	const Integer operator+(const Integer& lhs, const Integer& rhs)
	{
		return Integer(lhs) += rhs;
	}
	const Integer operator-(const Integer& lhs, const Integer& rhs)
	{
		return Integer(lhs) -= rhs;
	}

}//end namespace myInteger




int main()
{

	using namespace myInteger;
//	RB_Tree<string> rbt2;


	RB_Tree<string> rbtImpl1{ less<string>() };
	rbtImpl1.RB_Insert("aaa");



	map<int, int> sss;
	sss.insert(make_pair(1, 2));
	cout << sss[1] << endl;
	sss.insert(make_pair(11, 4));
	sss.insert(make_pair(2, 5));
	sss.insert(make_pair(3, 3));
	sss.insert_or_assign(1, 7);
	cout << sss[1] << endl;
	sss[1] = 65675;
	cout << sss[1] << endl;



	system("pause");
	return 0;
}
