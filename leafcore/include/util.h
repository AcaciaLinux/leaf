#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstddef>
#include <stdint.h>
#include <algorithm>
#include <set>
#include <deque>

#include "config.h"

namespace LeafUtil
{

    /**
     * @brief   Removes all the duplicates within the supplied deque
     * @tparam  T                   The type stored in the deque
     * @param   deque               The deque to remove the duplicates of
     * @return  size_t              The amount of duplicates removed
     */
    template <typename T> size_t    removeDuplicates(std::deque<T>& deque){
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

    /**
     * @brief   Converts the supplied byte count to a human readable count (KB, MB...)
     * @param   byteCount           The count to convert
     * @return  std::string
     */
    std::string                     bytesToHuman(uint64_t byteCount);

    /**
     * @brief   Parses the dependency string of a package list entry
     * @param   input               The list of dependencies in the form "[d1][d2]"
     * @return  A vector containing the names
     */
    std::deque<std::string>         parseDependenciesString(std::string input);

    /**
         * @brief   Asks the user for permission to do something in the format "question (y/n)"
         * @param   question        The question to ask
         * @param   defaultOption   If agreeing option should be checked or the negating option
         */
    bool                            askUserOK(const Leaf::config& conf, std::string question, bool defaultOption = true);

    /**
     * @brief   Executes a command, chroots into another root if necessary
     * @param   command             The command to execute
     * @param   workdir             The directory the command should be executed in
     * @return  The commands return value
     */
    int                             runCommand(const Leaf::config& conf, const std::string& command, const std::filesystem::path& workdir);

} // namespace LeafUtil

#endif
