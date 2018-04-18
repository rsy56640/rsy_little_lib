#pragma once
#ifndef _KMP_H
#define _KMP_H
#include <functional>
#include <string>
#include <vector>
#include <iterator>
#include <cassert>

namespace RSY_TOOL
{

	namespace KMP
	{


		/*
		 * Knuth-Morris-Pratt algorithm
		 * It's used to match the pattern string in the text.
		 * Time Complexity: O(n), n is the length of the text.
		**/
		//@ Parameter list:
		//@     text:      a long series of data.
		//@     pattern:   the pattern to be matched.
		//@ Return value:
		//@     return an array of the initio iterator of the matched string.
		std::vector<std::string::const_iterator>
			kmp(const std::string& text, const std::string& pattern);


		/*
		 * compute prefix function,
		 * which represents the maximum length of identical suffix and prefix.
		 * Time Complexity: O(m), m is the length of pattern.
		**/
		//@ Parameter list:
		//@     pattern:    the pattern to be matched.
		//@     prefix:     the prefix function.s
		//@ Return value:
		//@     No return value, and prefix is set appropriate.
		inline void
			kmp_prefix_function(const std::string& pattern, std::vector<int>& prefix);



		/*
		 * Generics program for KMP searching algorithm.
		 * Require random-access-iterator.
		**/

		/*
		 * verify whether the iterator is random accessable.
		**/

#ifndef _ISRANDOM_ACCESS
#define _ISRANDOM_ACCESS
		namespace
		{

			template<class IterT>
			struct isRandom_Access :std::false_type {};

			template<>
			struct isRandom_Access<std::random_access_iterator_tag> :std::true_type {};

			//no need, because template struct iterator_traits has specialized for _Ty* 
			//template<>
			//struct isRandom_Access<void*> :std::true_type {};

		}//end anonymous namespace
#endif // !_ISRANDOM_ACCESS

		/*
		 * KMP TEMPLATE
		**/
		//@ Parameter list:
		//@     _text_First:     the begin iterator of the text.
		//@     _text_Last:      the end iterator of the text.
		//@     _pattern_First:  the begin iterator of the pattern.
		//@     _pattern_Last:   the end iterator of the pattern.
		template<class _InIt>
		std::vector<_InIt>
			kmp_template(_InIt _text_First, _InIt _text_Last,
				_InIt _pattern_First, _InIt  _pattern_Last)
		{

			using difference_type = typename std::iterator_traits<_InIt>::difference_type;
			using value_type = typename std::iterator_traits<_InIt>::value_type;
			using pointer = typename std::iterator_traits<_InIt>::pointer;
			using reference = typename std::iterator_traits<_InIt>::reference;
			using iterator_category = typename std::iterator_traits<_InIt>::iterator_category;

			static_assert(
				isRandom_Access<iterator_category>::value,
				"Random Access Iterator required.\n");

			const difference_type _text_length = _text_Last - _text_First;
			const difference_type _pattern_length = _pattern_Last - _pattern_First;


			/*
			 * compute prefix function
			**/
			std::vector<difference_type> prefix(_pattern_length);
			prefix[0] = -1;

			//cur_pos must always be the backward last position of the prefix string 
			//that the previous prefix[i-1] matches.
			difference_type cur_pos = -1;

			//compute every prefix[i]
			for (difference_type i = 1; i < _pattern_length; ++i)
			{

				//0 1 2, ..., cur, cur+1, ..., i-1, i, ...
				//A B C  ...   X     Y
				//       A B C      ...         X   ?
				while (cur_pos >= 0 && *(_pattern_First + (cur_pos + 1)) != *(_pattern_First + i))
					cur_pos = prefix[cur_pos];

				if (*(_pattern_First + (cur_pos + 1)) == *(_pattern_First + i))
					cur_pos++;

				prefix[i] = cur_pos;

			}


			/*
			 * Knuth-Morris-Pratt algorithm
			 * It's used to match the pattern in the text.
			 * Time Complexity: O(n), n is the length of the text.
			**/
			std::vector<_InIt> result;

			int q = 0;			//number of characters matched

			for (auto it = _text_First; it != _text_Last; ++it)
			{

				while (q > 0 && *(_pattern_First + q) != *it)
					q = prefix[q - 1] + 1;

				if (*(_pattern_First + q) == *it)
					q++;

				if (q == _pattern_length)
				{
					result.push_back(it - (_pattern_length - 1));
					q = prefix[q - 1] + 1;
				}

			}

			return result;

		}//end TEMPLATE METHOD KMP


	}//end namespace KMP

}//end namespace RSY_TOOL

#endif // !_KMP_H
