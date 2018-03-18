#include "Binary_Indexed_Tree.h"
using namespace std;

namespace myInteger
{

	class Integer
	{
	public:

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

	using myInteger::Integer;

	vector<Integer> v = { 1,2,7,5,8 };

	Binary_Indexed_Tree<Integer> bit
	{ v, myInteger::operator+,Integer{ 0 },
		std::bind(myInteger::operator-, Integer{ 0 }, std::placeholders::_1) };

	Integer x, y, z;

	[[maybe_unused]]_STD vector<_STD pair<int, int> > errnum;

	try
	{

		x = bit.query(0, 2);

		bit.modify(2, 1);

		y = bit.query(0, 3);

		bit.modify(3, [](const Integer& a)->Integer {return Integer(a + 14); });

		z = bit.query(0, 4);

	}
	catch (BIT_Exception& e)
	{
		std::cout << e << std::endl;
	}


	system("pause");
	return 0;
}
