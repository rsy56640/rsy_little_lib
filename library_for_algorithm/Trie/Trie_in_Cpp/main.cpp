#include <iostream>
#include "Trie.hpp"
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;


void test()
{
	using namespace RSY_TOOL::Trie;
	Trie<char> trie;
	unordered_set<string> s;
	vector<string> strs = { "asd","ass","asdss","qwe","ewq","qweee","qwasd" };
	for (auto const& str : strs)
	{
		trie.insert(str.begin(), str.end());
		auto it = str.begin();

		using U = decltype(*str.begin());
		using T = std::decay_t<decltype(*str.begin())>;
		s.insert(str);
	}

}


int main()
{
	test();
	system("pause");
	return 0;
}