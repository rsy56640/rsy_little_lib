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
	bool OK = true;
	vector<string> strs = { "asd","ass","asdss","qwe","ewq","qweee","qwasd" };
	for (auto const& str : strs)
	{
		trie.insert(str.begin(), str.end());
		s.insert(str);
	}
	cout << trie.find("asd") << " " << (s.find("asd") != s.end()) << endl;
	if (trie.find("asd") != (s.find("asd") != s.end())) OK = false;
	trie.erase("asdss");
	s.erase("asdss");
	if (trie.find("asdss") != (s.find("asdss") != s.end())) OK = false;
	trie.insert("asdss");
	s.insert("asdss");
	s.erase("asd");
	trie.erase("asd");
	if (trie.find("asdss") != (s.find("asdss") != s.end())) OK = false;
	trie.erase("qwe");
	s.erase("qwe");
	if (trie.find("qwe") != (s.find("qwe") != s.end())) OK = false;
	if (trie.find("qweee") != (s.find("qweee") != s.end())) OK = false;
	if (OK)cout << "success" << endl;
	else cout << "fail" << endl;
}


int main()
{
	test();
	system("pause");
	return 0;
}