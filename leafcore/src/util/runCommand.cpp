/**
 * @file        util/runCommand.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of LeafUtil::runCommand()
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "util.h"
#include "leaffs.h"

#include <filesystem>

static std::string token_rootdir = "{ROOTDIR}";
static std::string token_command = "{COMMAND}";

int LeafUtil::runCommand(const Leaf::config& conf, const std::string& command, const std::filesystem::path& workdir) {
    FUN();
    LEAF_DEBUG_EX("LeafUtil::runCommand()");

    bool shouldChroot = conf.root != "/";

    std::string execCMD = command;
    std::string execWorkdir = workdir;

    if (shouldChroot){
        //Use the chroot command for executing this command
        execCMD = conf.chroot_cmd;
        execCMD.replace(execCMD.find(token_rootdir), token_rootdir.length(), conf.root);
        execCMD.replace(execCMD.find(token_command), token_command.length(), command);
        LEAF_DEBUG_EX_ADD("LeafUtil::runCommand_execCMD", execCMD);

        execWorkdir = std::filesystem::relative(execWorkdir, conf.root);
        LEAF_DEBUG_EX_ADD("LeafUtil::runCommand_execWorkdir", execWorkdir);
    }

    LOGI("[LeafUtil][runCommand] Running command \"" + execCMD + "\" in " + execWorkdir);

    int res = 0;
    {//Switch the workdir, execute the command and switch back
        std::string oldWorkDir;
        if (!shouldChroot){
            oldWorkDir = LeafFS::get_workdir();
            LeafFS::change_workdir(execWorkdir);
        } else {
            LOGI("[LeafUtil][runCommand] Skipping workdir change when chrooting");
        }

        res = system(execCMD.c_str());

        if (!shouldChroot)
            LeafFS::change_workdir(oldWorkDir);
    }

    return res;
}
