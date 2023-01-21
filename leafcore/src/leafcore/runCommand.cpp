/**
 * @file        leafcore/runCommand.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of Leafcore::runCommand()
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leafcore.h"
#include "leaffs.h"

#include <filesystem>

static std::string token_rootdir = "{ROOTDIR}";
static std::string token_command = "{COMMAND}";

int Leafcore::runCommand(const std::string& command, const std::filesystem::path& workdir) const {
    FUN();
    LEAF_DEBUG_EX("Leafcore::runCommand()");

    bool shouldChroot = _config.rootDir != "/";

    std::string execCMD = command;
    std::string execWorkdir = workdir;

    if (shouldChroot){
        //Use the chroot command for executing this command
        execCMD = _config.chroot_cmd;
        execCMD.replace(execCMD.find(token_rootdir), token_rootdir.length(), _config.rootDir);
        execCMD.replace(execCMD.find(token_command), token_command.length(), command);
        LEAF_DEBUG_EX_ADD("Leafcore::runCommand_execCMD", execCMD);

        execWorkdir = std::filesystem::relative(execWorkdir, _config.rootDir);
        LEAF_DEBUG_EX_ADD("Leafcore::runCommand_execWorkdir", execWorkdir);
    }

    LOGI("[Leafcore][runCommand] Running command \"" + execCMD + "\" in " + execWorkdir);

    int res = 0;
    {//Switch the workdir, execute the command and switch back
        std::string oldWorkDir = LeafFS::get_workdir();
        LeafFS::change_workdir(execWorkdir);

        res = system(execCMD.c_str());

        LeafFS::change_workdir(oldWorkDir);
    }

    return res;
}
