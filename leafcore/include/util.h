#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstddef>
#include <stdint.h>
#include <algorithm>
#include <set>
#include <deque>

namespace LeafUtil
{
	
	/**
	 * @brief	Removes all the duplicates within the supplied deque
	 * @tparam	T					The type stored in the deque
	 * @param	deque				The deque to remove the duplicates of 
	 * @return	size_t				The amount of duplicates removed
	 */
	template <typename T> size_t	removeDuplicates(std::deque<T>& deque){
		std::set<T> seen;

		auto newEnd = std::remove_if(deque.begin(), deque.end(), [&seen](const T& value)
		{
			if (seen.find(value) != std::end(seen))
				return true;

			seen.insert(value);
			return false;
		});

		deque.erase(newEnd, deque.end());

		return deque.size();
	}

	namespace Progress{
		void init();
		void print(std::string prefix, uint64_t total, uint64_t progress, uint64_t steps, std::string preProgress = "");
		void end();
	} // namespace Progress


} // namespace LeafUtil




#endif
