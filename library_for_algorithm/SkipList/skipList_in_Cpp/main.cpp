#include <iostream>
#include "SkipList.hpp"
#include <string>
#include <vector>
#include <map>
#include <random>
#include <ctime>
#include <Windows.h>
using namespace std;
using namespace RSY_TOOL::SkipList;


void test()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	SkipList<int, int> sl([](const int& lhs, const int& rhs) { return lhs < rhs; });
	map<int, int> mp;
	constexpr int times = 100000;
	constexpr int range = 10000;
	srand(static_cast<std::size_t>(time(0)));
	bool flag = true;
	DWORD  map_time = 0u, skip_time = 0u;
	DWORD  t;
	try {
		for (int i = 0; i < times; i++)
		{
			cout << i << " ";
			int k = rand() % range;
			int v = rand() % range;
			if (i % 4 == 0)
			{
				t = GetTickCount();
				mp.insert(make_pair(k, v));
				map_time += GetTickCount() - t;

				t = GetTickCount();
				sl.insert(k, v);
				skip_time += GetTickCount() - t;
			}
			if (i % 4 == 1)
			{
				t = GetTickCount();
				mp.insert_or_assign(k, v);
				map_time += GetTickCount() - t;

				t = GetTickCount();
				sl.insert_or_assign(k, v);
				skip_time += GetTickCount() - t;
			}
			if (i % 4 == 2)
			{
				t = GetTickCount();
				mp.erase(k);
				map_time += GetTickCount() - t;

				t = GetTickCount();
				sl.erase(k);
				skip_time += GetTickCount() - t;
			}
			if (i % 4 == 3)
			{
				t = GetTickCount();
				mp[k] = mp[v];
				map_time += GetTickCount() - t;

				t = GetTickCount();
				sl[k] = sl[v];
				skip_time += GetTickCount() - t;
			}
		}
		cout << endl;
		cout << "map size: " << mp.size() << endl;
		cout << "skiplist size: " << sl.size() << endl;
		for (auto it : mp)
		{
			if (sl[it.first] != it.second)
			{
				cout << "not equal: " << it.first << " " << it.second << endl;
				flag = false;
			}
		}
	}
	catch (std::out_of_range&)
	{
		cout << "std::out_of_range" << endl;
	}
	if (flag) cout << "success" << endl;
	cout << "map time: " << map_time << endl;
	cout << "skiplist time: " << skip_time << endl;
}


int main()
{
	test();
	system("pause");
	return 0;
}