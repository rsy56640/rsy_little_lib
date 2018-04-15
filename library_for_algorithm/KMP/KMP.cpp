#include "KMP.h"

namespace RSY_TOOL
{

	namespace KMP
	{

		std::vector<std::string::const_iterator>
			kmp(const std::string& text, const std::string& pattern)
		{

			const std::size_t text_size = text.size();
			const std::size_t pattern_size = pattern.size();
			std::vector<int> prefix(pattern_size);
			kmp_prefix_function(pattern, prefix);

			//in fact, difference_type is int.
			using difference_type =
				typename std::iterator_traits<std::string::iterator>::difference_type;

			int q = 0;			//number of characters matched

			std::vector<std::string::const_iterator> result;

			for (auto it = text.begin(); it != text.end(); ++it)
			{

				while (q > 0 && pattern[q] != *it)
					q = prefix[q - 1] + 1;

				if (pattern[q] == *it)
					q++;

				if (q == pattern_size)
				{
					result.push_back(it - (pattern_size - 1));
					q = prefix[q - 1] + 1;
				}

			}

			return result;

		}//end function kmp();


		void kmp_prefix_function(const std::string& pattern, std::vector<int>& prefix)
		{

			const std::size_t size = pattern.size();
			prefix[0] = -1;

			//cur_pos must always be the last position of the prefix string 
			//that the previous prefix[i-1] matches.
			int cur_pos = -1;

			//compute every prefix[i]
			for (std::size_t i = 1; i < size; ++i)
			{

				//0 1 2, ..., cur, cur+1, ..., i-1, i, ...
				//A B C  ...   X     Y
				//       A B C      ...         X   ?
				while (cur_pos >= 0 && pattern[cur_pos + 1] != pattern[i])
					cur_pos = prefix[cur_pos];

				if (pattern[cur_pos + 1] == pattern[i])
					cur_pos++;

				prefix[i] = cur_pos;

			}

		}//end function kmp_prefix_function();


	}//end namespace KMP

}//end namespace RSY_TOOL
