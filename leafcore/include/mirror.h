#ifndef __LEAF_MIRROR_H__
#define __LEAF_MIRROR_H__

namespace Leaf {
    class Mirror;
}

#include <string>
#include <memory>

#include "config.h"
#include "package.h"

namespace Leaf {

    class Mirror{

    public:

        /**
         * @brief   Creates a new mirror
         * @param   name            The name for the new mirror
         * @param   url             The url to fetch the package list from
         */
        Mirror(const std::string& name, const std::string& url);

        /**
         * @brief   Updates the mirrors local cache for up-to-date lists
         * @param   conf            The update config to process
         */
        void                        update(const conf_update& conf);

        /**
         * @brief   Searches for a package with the supplied name in this mirror
         * @param   name            The name to search for
         * @return  nullptr if not found
         * @throws  LeafError if the mirror has not been loaded using load()
         */
        std::shared_ptr<Package>    resolve_package(const std::string& name) const;

        /**
         * @brief   Loads the current mirror cache from the persistent file
         * @param   conf            The config to process
         * @throws  LeafError if the file has not been found
         */
        void                        load(const Leaf::config& conf);

        /**
         * @brief   Returns if the mirror cache is loaded
         */
        bool                        loaded() const;

    private:

        /// @brief  The name for the mirror
        std::string                 _name;

        /// @brief  The url to fetch the package list from
        std::string                 _url;

        /// @brief  All the packages that are from this mirror (only when loaded)
        std::deque<std::shared_ptr<Package>> _packages;
        bool                        _is_loaded = false;
    };

}

#endif