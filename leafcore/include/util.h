#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstddef>
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

} // namespace LeafUtil




#endif
