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
	int size = pattern.size();

	vector<int> prefix(size);
	kmp_prefix_function(pattern, prefix);
	auto result = kmp(text, pattern);

	for (auto it = result.begin(); it != result.end(); ++it)
	{
		std::copy(*it, *it + pattern.size(), _oit);
		cout << endl;
	}



	system("pause");
	return 0;
}
