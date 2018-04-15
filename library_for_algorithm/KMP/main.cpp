#include <iostream>
#include "KMP.h"

#include <string>
#include <numeric>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace RSY_TOOL::KMP;

std::ostream_iterator<char> _oit(std::cout, "");


int main()
{

	string pattern = "abxab";
	string text = "abxcabxabaxabxababx";


	auto result = kmp(text, pattern);
	for (auto it = result.begin(); it != result.end(); ++it)
	{
		std::copy(*it, *it + pattern.size(), _oit);
		cout << endl;
	}


	cout << "\n\n" << endl;


	auto result2 = kmp_template(text.begin(), text.end(), pattern.begin(), pattern.end());
	for (auto it = result2.begin(); it != result2.end(); ++it)
	{
		std::copy(*it, *it + pattern.size(), _oit);
		cout << endl;
	}


	system("pause");
	return 0;
}
