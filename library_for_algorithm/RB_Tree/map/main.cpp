#include "rsy_map.h"
#include <set>
#include <map>
#include <functional>
#include <vector>
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

		const int value()const { return this->val; }

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

namespace std
{
	using myInteger::Integer;
	template<>
	struct less<Integer>
	{

		bool operator()(const Integer& lhs, const Integer& rhs)
		{
			return lhs.value() < rhs.value();
		}
	};
}


int main()
{



	vector<int>::iterator it;

	map<int, int> m;

	rsy_map<int, int> rm;

	RB_Tree_Iterator_Base<int> a;

	RB_Tree<string> rbtImpl1{ less<string>() };
	try
	{


		rbtImpl1.insert("aaa");
		rbtImpl1.insert("cbb");
		rbtImpl1.insert("sss");
		rbtImpl1.insert("bbf");
		rbtImpl1.insert("aaf");
		rbtImpl1.insert("sas");

		rbtImpl1.erase("cbb");
		rbtImpl1.erase("ssb");

	}
	catch (RB_Tree_Exception& e)
	{
		cout << e << endl;
	}



	system("pause");
	return 0;
}
