#pragma once
#ifndef _KMP2_H
#define _KMP2_H
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
		 * Generics program for KMP searching algorithm.
		 * Require random-access-iterator.
		**/

		/*
		 * verify whether the iterator is random accessable.
		**/

#ifndef _IS_RANDOM_ACCESS
#define _IS_RANDOM_ACCESS
		namespace
		{

			template<class IterT>
			struct isRandom_Access :std::false_type {};

			template<>
			struct isRandom_Access<std::random_access_iterator_tag> :std::true_type {};

		}//end anonymous namespace
#endif // !_IS_RANDOM_ACCESS

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
			kmp_template2(_InIt _text_First, _InIt _text_Last,
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
			std::vector<_InIt> prefix(_pattern_length);
			prefix[0] = _pattern_First;

			//cur_pos must always be the backward last position of the prefix string 
			//that the previous prefix[i-1] matches.
			difference_type cur_pos = 0;

			//compute every prefix[i]
			for (difference_type i = 1; i < _pattern_length; ++i)
			{

				//0 1 2, ..., cur, cur+1, ..., i-1, i, ...
				//A B C  ...   X     Y
				//       A B C      ...         X   ?
				while (cur_pos > 0 && *(_pattern_First + cur_pos) != *(_pattern_First + i))
					cur_pos = prefix[cur_pos] - _pattern_First;

				if (*(_pattern_First + cur_pos) == *(_pattern_First + i))
					cur_pos++;

				prefix[i] = _pattern_First + cur_pos;

			}


			/*
			 * Knuth-Morris-Pratt algorithm
			 * It's used to match the pattern in the text.
			 * Time Complexity: O(n), n is the length of the text.
			**/
			std::vector<_InIt> result;

			difference_type q = 0;			//number of elements matched

			for (auto it = _text_First; it != _text_Last; ++it)
			{

				while (q > 0 && *(_pattern_First + q) != *it)
					q = prefix[q - 1] - prefix[0];

				if (*(_pattern_First + q) == *it)
					q++;

				if (q == _pattern_length)
				{
					result.push_back(it - (_pattern_length - 1));
					q = prefix[q - 1] - prefix[0];
				}

			}

			return result;

		}//end TEMPLATE METHOD KMP


	}//end namespace KMP

}//end namespace RSY_TOOL

#endif // !_KMP2_H
