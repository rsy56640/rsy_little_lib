#include "rsy_map.h"
#include <set>
#include <map>
#include <functional>
#include <iostream>
#include <vector>
#include <cassert>
using namespace RSY_TOOL::MY_RB_Tree;
using namespace std;

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

template<class Key, class Value>
void print(const rsy_map<Key, Value>& map)
{
	for (typename rsy_map<Key, Value>::iterator it = map.begin(); it != map.end(); ++it)
	{
		cout << it->second << endl;
	}
	cout << endl;
}

class myStr {
public:
	/*
	myStr()
		:_s{}
	{}
	*/
	myStr(string s)
		:_s(s)
	{}
	myStr(const myStr&) = default;
	myStr& operator=(const myStr&) & = default;

private:
	string _s;
};

int main()
{

	ostream_iterator<int> _oit(cout, " ");

	rsy_map<int, string>::iterator it;


	map<int, myStr> m;
	m.insert_or_assign(1, string("123"));
	m.erase(0);
	//myStr sss = m[0];		//require default constructor

	rsy_map<int, string> rbtImpl1;

	//RB_Tree<string> rbtImpl1{ less<string>() };

	try
	{

		rbtImpl1.insert(make_pair(5, "aaa"));
		rbtImpl1.insert(make_pair(2, "cbb"));
		rbtImpl1.insert(make_pair(1, "sss"));
		rbtImpl1.insert(make_pair(4, "bbf"));
		rbtImpl1.insert(make_pair(6, "aaf"));
		rbtImpl1.insert(make_pair(3, "sas"));
		rbtImpl1.insert(make_pair(14, " ^*&"));

		print(rbtImpl1);

		auto it1 = rbtImpl1.find(5);
		if (it1 != rbtImpl1.end())
			cout << (*it1).second << endl;

		rbtImpl1.erase(1);
		rbtImpl1.erase(2);

		cout << endl;

		print(rbtImpl1);

	}
	catch (RB_Tree_Exception& e)
	{
		cout << e << endl;
	}



	system("pause");
	return 0;
}
