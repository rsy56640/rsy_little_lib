#include "KMP.h"
#include "KMP2.h"

#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace RSY_TOOL::KMP;

std::ostream_iterator<char> _oit(std::cout, "");


int main()
{

	string pattern = "abs";
	string text = "absxxccabcsabseabs";
	char* p = "abs";
	char* t = "absxxccabcsabseabs";


	auto result = kmp(text, pattern);
	for (auto it = result.begin(); it != result.end(); ++it)
	{
		std::copy(*it, *it + pattern.size(), _oit);
		cout << "\t" << *it - text.begin() << endl;
	}


	cout << "\n\n" << endl;


	auto result1 = kmp_template(text.begin(), text.end(), pattern.begin(), pattern.end());
	for (auto it = result1.begin(); it != result1.end(); ++it)
	{
		std::copy(*it, *it + pattern.size(), _oit);
		cout << "\t" << *it - text.begin() << endl;
	}


	cout << "\n\n" << endl;


	auto result2 = kmp_template2(text.begin(), text.end(), pattern.begin(), pattern.end());
	for (auto it = result2.begin(); it != result2.end(); ++it)
	{
		std::copy(*it, *it + pattern.size(), _oit);
		cout << "\t" << *it - text.begin() << endl;
	}


	cout << "\n\n" << endl;


	auto result3 = kmp_template2(t, t + strlen(t), p, p + strlen(p));
	for (auto it = result3.begin(); it != result3.end(); ++it)
	{
		std::copy(*it, *it + strlen(p), _oit);
		cout << "\t" << *it - t << endl;
	}


	system("pause");
	return 0;
}
