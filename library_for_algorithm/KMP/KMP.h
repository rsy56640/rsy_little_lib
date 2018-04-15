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
		//@     text:
		//@     pattern:
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
		//@     pattern:
		//@ Return value:
		//@     No return value, and prefix is set appropriate.
		inline void
			kmp_prefix_function(const std::string& pattern, std::vector<int>& prefix);



		/*
		 * verify whether the iterator is random accessable.
		**/
		namespace
		{

			template<class IterT>
			struct isRandom_Access :std::false_type {};

			template<>
			struct isRandom_Access<std::random_access_iterator_tag> :std::true_type {};

		}//end anonymous namespace

		/*
		 * KMP TEMPLATE
		 *
		**/
		//@ Parameter list:
		//@     _text_First:
		//@     _text_Last:
		//@     _pattern_First:
		//@     _pattern_Last:
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


			//compute prefix function








			std::vector<_InIt> result;

			return result;
		}


	}//end namespace KMP

}//end namespace RSY_TOOL

#endif // !_KMP_H
