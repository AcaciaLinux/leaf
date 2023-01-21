#include "../t_leafcore.h"

#include "leaffs.h"

#include <filesystem>
#include <fstream>
#include <iterator>

TEST(Leafcore, runCommand_debug){
    FUN();

    LEAF_DEBUG_SET_FAIL("Leafcore::runCommand()");

    try{
        Leafcore core;
        core.runCommand("/bin/ls", core.getConfig().rootDir);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Leafcore, runCommand_execCMD){
    FUN();

    LEAF_DEBUG_SET_FAIL("Leafcore::runCommand_execCMD");

    try{
        Leafcore core;
        core.getConfig().rootDir = "someRandomRootDir";
        core.getConfig().chroot_cmd = "chroot {ROOTDIR} {COMMAND}";
        core.runCommand("/bin/ls", core.getConfig().rootDir);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
        ASSERT_EQ("chroot someRandomRootDir /bin/ls", e->getAdditional());
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Leafcore, runCommand_execWorkdir){
    FUN();

    LEAF_DEBUG_SET_FAIL("Leafcore::runCommand_execWorkdir");

    try{
        Leafcore core;
        core.getConfig().rootDir = "/my/someRandomRootDir/";
        core.runCommand("/bin/ls", core.getConfig().rootDir + "root/");

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
        ASSERT_EQ("root/", e->getAdditional());
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}
