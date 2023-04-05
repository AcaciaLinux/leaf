#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <deque>
#include <istream>
#include <map>

#include "mirror.h"

#define LEAF_CONFIGFILE_DEFAULT "/etc/leaf.conf"

namespace Leaf{

    /**
     * @brief   Enumerates the different types of actions leaf can perform
     */
    enum action_type {
        A_UNKNOWN = 0,
        A_UPDATE = 1,
        A_TRANSACTION = 2
    };

    /**
     * @brief   Enumerates the different types of transactions
     */
    enum conf_tr_type {
        TR_UNKNOWN = 0,
        TR_INSTALL = 1,     //Install a package to the system
        TR_CHANGE = 2,      //Change a installed package
        TR_REMOVE = 3,      //Remove a package from the system
    };

    /**
     * @brief   A parent struct for managing actions in the config
     */
    struct conf_action {
        action_type                 a_type = A_UNKNOWN;
    };

    /**
     * @brief   An action that represents a transaction (A_TRANSACTION)
     */
    struct conf_tr : conf_action{

        conf_tr() {
            a_type = A_TRANSACTION;
        }

        /// @brief  The type of the transaction
        conf_tr_type                tr_type = TR_UNKNOWN;

        /// @brief  If some leaf should just try to complete the transaction no matter what happens
        bool                        force = false;

        /// @brief  If pre/postinstall scripts should be run
        bool                        runScripts = true;

        /// @brief  The names for the packages to process
        std::deque<std::string>     packages;
    };

    struct conf_tr_install : conf_tr {
        conf_tr_install() {
            tr_type = TR_INSTALL;
        }
    };

    struct conf_tr_change : conf_tr {
        conf_tr_change() {
            tr_type = TR_CHANGE;
        }
    };

    struct conf_tr_remove : conf_tr {
        conf_tr_remove() {
            tr_type = TR_REMOVE;
        }
    };

    /**
     * @brief   The configuration leaf should process
     */
    struct config {

        /// @brief  The root to operate on
        std::string                 root;

        /// @brief  The actions to perform
        std::deque<conf_action>     actions;

        /// @brief  The list of mirrors to check for packages
        std::deque<Mirror>          mirrors;
    };

    bool parse_config(config& conf, std::istream& instream);

}

#endif
