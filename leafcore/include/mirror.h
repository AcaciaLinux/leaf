#ifndef __LEAF_MIRROR_H__
#define __LEAF_MIRROR_H__

#include <string>

namespace Leaf {

    class Mirror{

    public:

        /**
         * @brief   Creates a new mirror
         * @param   name            The name for the new mirror
         * @param   url             The url to fetch the package list from
         */
        Mirror(const std::string& name, const std::string& url);

    private:

        /// @brief  The name for the mirror
        std::string                 _name;

        /// @brief  The url to fetch the package list from
        std::string                 _url;

    };

}

#endif