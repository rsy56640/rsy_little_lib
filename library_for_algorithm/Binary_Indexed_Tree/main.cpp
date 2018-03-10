#include "Binary_Indexed_Tree.h"
using namespace std;


class Integer
{
public:

	Integer(int x = 0) :val(x) {}

	Integer(const Integer& other) :val(other.val) {}

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


int main()
{

	vector<Integer> v = { Integer(1),Integer(2),Integer(7),Integer(5),Integer(8) };

	Binary_Indexed_Tree<Integer> bit
	(v, [](const Integer& a, const Integer& b)->Integer { return a + b; },
		0, [](const Integer& a) {return Integer(0) - a; });

	Integer x, y, z;

	try
	{

		x = bit.query(0, 2);

		bit.modify(2, 1);

		y = bit.query(0, 3);

		bit.modify(3, [](Integer& a) {a += 14; });

		z = bit.query(0, 4);

	}
	catch (BIT_Exception& e)
	{
		std::cout << e << std::endl;
	}


	system("pause");
	return 0;
}