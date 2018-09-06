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
	using _InIt = string::iterator;
	using Key = char;
	constexpr auto b = std::is_convertible_v<
		typename std::iterator_traits<_InIt>::iterator_category,
		std::forward_iterator_tag> &&
		std::is_convertible_v<
		std::decay_t<RSY_TOOL::Trie::_InIt_Element_t<_InIt>>,
		Key>;
	test();
	system("pause");
	return 0;
}