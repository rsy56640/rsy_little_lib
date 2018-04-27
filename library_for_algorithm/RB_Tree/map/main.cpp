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
		cout << it->first << "\t" << it->second << endl;
	}
	cout << endl;
}


class myStr {
public:
	///*
	myStr()
		:_s{}
	{}
	//*/
	myStr(string s)
		:_s(s)
	{}
	myStr(const myStr&) = default;
	myStr& operator=(const myStr&) & = default;

	operator string() const
	{
		return _s;
	}

private:
	string _s;
	friend ostream& operator<<(ostream&, const myStr&);
};
ostream& operator<<(ostream& os, const myStr& ms)
{
	os << ms._s;
	return os;
}

template<class T, class U>
class Fuck
{
public:
	Fuck(U a) :f(a) {}

	class Fuck2
	{

	public:

		Fuck2(U a) :x(a) {}

		operator U() const
		{
			return static_cast<U>(x);
		}

		U x;

	};



	Fuck2 get()
	{
		return f;
	}

	operator Fuck2()
	{
		return f;
	}

private:

	Fuck2 f;

};

void printStr(const string& str)
{
	cout << str << endl;
}


ostream& operator<<(ostream& os, const string& s)
{
	std::operator<<(os, s);
	return os;
}


int main()
{

	ostream_iterator<int> _oit(cout, " ");


	map<int, myStr> m;
	m.insert_or_assign(1, string("123"));
	m.erase(0);
	m[2] = string("qwe");
	m.at(1) = string("asd");
	myStr sss = m[0];		//require default constructor



	Fuck<string, myStr> a{ string("22") };
	myStr b = a.operator Fuck<string, myStr>::Fuck2().operator myStr();
	myStr c = a.operator Fuck<string, myStr>::Fuck2();
	myStr d = (a.get());
	cout << d << endl;
	cout << a.get() << endl;

	rsy_map<int, std::string> mmp;

	cout << typeid(rsy_map<int, string>::iterator_category).name() << endl;
	cout << typeid(rsy_map<int, string>::iterator_type).name() << endl;
	cout << endl;


	try
	{

		mmp.insert(make_pair(5, "aaa"));
		mmp.insert(make_pair(2, "cbb"));
		mmp.insert(make_pair(1, "sss"));
		mmp.insert(make_pair(4, "bbf"));
		mmp.insert(make_pair(6, "aaf"));
		mmp.insert(make_pair(3, "sas"));
		mmp.insert(make_pair(14, " ^*&"));

		print(mmp);

		auto it1 = mmp.find(5);
		if (it1 != mmp.end())
			cout << (*it1).second << endl;

		mmp.erase(1);
		mmp.erase(2);

		mmp[6] = "tyu";
		mmp[2] = mmp[6];

		cout << endl;

		mmp[6] = "rsy";

		try {
			string s = mmp.at(7);
		}
		catch (std::out_of_range& e)
		{
			cout << e.what() << endl;
		}

		string sss = mmp[6];
		cout << sss << endl;
		printStr(mmp[6]);

		ostream& operator<<(ostream&, const string&);     //WTF !!!!
		cout << mmp[6] << endl;                           //WTF !!!!



		cout << mmp[6].operator string() << endl;
		cout << mmp[2].operator std::string() << endl;

		cout << endl;

		print(mmp);

		mmp[8] = "123";
		mmp[11] = "iop";
		mmp[12] = "uuu";

		auto it_begin = mmp.lower_bound(2);
		auto it_end = mmp.upper_bound(13);

		for (auto it = it_begin; it != it_end; ++it)
		{
			cout << it->first << "\t" << it->second << endl;
		}
		cout << endl;

		print(mmp);

	}
	catch (map_Exception& e)
	{
		cout << e << endl;
	}



	system("pause");
	return 0;
}
