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
	/*
	 * Notice that string literal is different from std::string,
	 * since and std::end(c) implies different char*.
	 */
	if (trie.find("asd"s) != (s.find("asd") != s.end())) OK = false;
	trie.erase("asdss"s);
	s.erase("asdss");
	if (trie.find("asdss"s) != (s.find("asdss") != s.end())) OK = false;
	trie.insert("asdss"s);
	s.insert("asdss");
	trie.erase("asd"s);
	s.erase("asd");
	if (trie.find("asdss"s) != (s.find("asdss") != s.end())) OK = false;
	trie.erase("qwe"s);
	s.erase("qwe");
	if (trie.find("qwe"s) != (s.find("qwe") != s.end())) OK = false;
	if (trie.find("qweee"s) != (s.find("qweee") != s.end())) OK = false;
	if (OK)cout << "success" << endl;
	else cout << "fail" << endl;
}


int main()
{
	test();
	system("pause");
	return 0;
}