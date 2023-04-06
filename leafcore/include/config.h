#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <deque>
#include <istream>
#include <map>
#include <filesystem>

// Forward declarations for the structs
namespace Leaf{
    struct conf_action;

    struct conf_update;
    struct conf_tr;

    struct conf_tr_install;
    struct conf_tr_change;
    struct conf_tr_remove;

    struct config;
}

#include "mirror.h"
#include "package.h"

#define LEAF_CONFIGFILE_DEFAULT "/etc/leaf.conf"

namespace Leaf{

    /// @brief  Enumerates the different types of actions leaf can perform
    enum action_type {
        A_UNKNOWN = 0,
        A_UPDATE = 1,           //Update is not  transaction, treat it separately
        A_TRANSACTION = 2       //Transations (install, remove...)
    };

    /// @brief  Enumerates the different types of asking behaviour
    enum ask_type {
        ASK_ASK,                //Ask the user normally
        ASK_DEFAULT,            //Assume the default
        ASK_YES,                //Just say yes
        ASK_NO                  //Just say no
    };

    /// @brief  Enumerates the different types of transactions
    enum conf_tr_type {
        TR_UNKNOWN = 0,
        TR_NONE = 1,            //Do nothing, for Hook
        TR_INSTALL = 2,         //Install a package to the system
        TR_CHANGE = 3,          //Change a installed package
        TR_REMOVE = 4,          //Remove a package from the system
    };

    /// @brief  Enumerates the different redownload behaviours
    enum conf_tr_redownload {
        REDOWNLOAD_NONE,        //Redownload only those packages that have differing hashes
        REDOWNLOAD_SPECIFIED,   //Redownload packages that are supplied to the command
        REDOWNLOAD_ALL          //Invalidate all download caches and redownload everything
    };

    /// @brief  A parent struct for managing actions in the config
    struct conf_action {
        action_type                 a_type = A_UNKNOWN;
    };

    /// @brief  Describes an update action (A_UPDATE)
    struct conf_update : conf_action{
        conf_update(config& parent) : parent(parent) {
            a_type = A_UPDATE;
        }

        /// @brief  The parent config
        config&                     parent;
    };

    /// @brief  An action that represents a transaction (A_TRANSACTION)
    struct conf_tr : conf_action{
        conf_tr(config& parent) : parent(parent){
            a_type = A_TRANSACTION;
        }

        /// @brief  The type of the transaction
        conf_tr_type                tr_type = TR_UNKNOWN;

        /// @brief  The parent config
        config&                     parent;

        /// @brief  If some leaf should just try to complete the transaction no matter what happens
        bool                        force = false;

        /// @brief  If leaf should clean its caches after the transaction finished
        bool                        clean = true;

        /// @brief  The names for the packages to process
        std::deque<std::string>     packages;

        /// @brief  The resolved package objects, for internal use only
        std::deque<Package*>        packages_;

        /// @brief  Regulate the redownload behaviour
        conf_tr_redownload          redownload = REDOWNLOAD_NONE;
    };

    /// @brief  Represents an install transaction
    struct conf_tr_install : conf_tr {
        conf_tr_install(config& conf) : conf_tr(conf) {
            tr_type = TR_INSTALL;
        }

        /// @brief  If the transaction should resolve and install dependencies
        bool                        installDependencies = true;
    };

    /// @brief  Represents a change transaction
    struct conf_tr_change : conf_tr {
        conf_tr_change(config& conf) : conf_tr(conf) {
            tr_type = TR_CHANGE;
        }

        /// @brief  If this transaction should respect (not break) dependencies or just force
        bool                        respectDependencies = true;

        /// @brief  If the remote hash should be considered to determine if a change is necessary
        bool                        checkRemoteHash = false;
    };

    /// @brief  Represents a remove transaction
    struct conf_tr_remove : conf_tr {
        conf_tr_remove(config& conf) : conf_tr(conf) {
            tr_type = TR_REMOVE;
        }
    };

    /// @brief  The main configuration leaf should process
    struct config {

        /// @brief  The root to operate on
        std::filesystem::path       root = "/";

        /// @brief  The actions to perform
        std::deque<conf_action>     actions;

        /// @brief  The list of mirrors to check for packages
        std::deque<Mirror>          mirrors;

        /// @brief  The behaviour when taking user questions
        ask_type                    ask = ASK_ASK;

        /// @brief  If progress bars should be displayed
        bool                        showProgress = true;

        /// @brief  If pre/postinstall/... scripts should be run
        bool                        runScripts = true;

        /// @brief  The command to use to chroot when executing commands in an other root
        std::string                 chroot_cmd = "chroot {ROOTDIR} {COMMAND}";

        //
        // ROOT - These directories govern all the following directories
        //

        /// @brief  An external directory for caches
        std::filesystem::path       extCacheDir = "";
        /// @brief  Returns the cache directory
        std::filesystem::path       cacheDir() const;

        /// @brief  An external directory for persistent / config data
        std::filesystem::path       extConfDir = "";
        /// @brief  Returns the configuration directory
        std::filesystem::path       configDir() const;

        //
        // CACHES - These caches can be external for performance and are not critical to persist
        //

        /// @brief  An external download cache (overrides extCacheDir)
        std::filesystem::path       extDLCache = "";
        /// @brief  Returns the download cache directory
        std::filesystem::path       downloadDir() const;

        /// @brief  An external package cache (overrides extCacheDir)
        std::filesystem::path       extPKGCache = "";
        /// @brief  Returns the package cache directory
        std::filesystem::path       packagesDir() const;

        /// @brief  An external run cache (overrides extCacheDir)
        std::filesystem::path       extRUNCache = "";
        /// @brief  Returns the run cache directory
        std::filesystem::path       runDir() const;

        //
        // PERSISTENT - These directories are persistent, installed files and hooks are placed in here
        //

        /// @brief  An external directory for installed packages (overrides extConfDir)
        std::filesystem::path       extInstalledDir = "";
        /// @brief  Returns the directory for .leafinstallled files
        std::filesystem::path       installedDir() const;

        /// @brief  An external directory for hooks (overrides extConfDir)
        std::filesystem::path       extHooksDir = "";
        /// @brief  Returns the directory to use for hooks
        std::filesystem::path       hooksDir() const;

        /// @brief  An external director for mirror caches (overrides extConfDir)
        std::filesystem::path       extMirrorsDir = "";
        /// @brief  Returns the directory to use for storing mirrors
        std::filesystem::path       mirrorsDir() const;

    };

    bool parse_config(config& conf, std::istream& instream);

}

#endif
