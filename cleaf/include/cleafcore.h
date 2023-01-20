/**
 * @file        cleafcore.h
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       A C wrapper for the Leafcore class and its functions
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#ifndef __CLEAFCORE_H__
#define __CLEAFCORE_H__

#include <stdint.h>

#define CLEAFCORE_OK 0
#define CLEAFCORE_NOTINIT -1
#define CLEAFCORE_NOCORE -2
#define CLAEFCORE_LEAFERROR -3

/**
 * @brief   A structure holding a Leafcore and LeafError object
 */
struct cleafcore{
    void*		core;
    void*		error;
};

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief   Creates a new Leafcore instance
     * @return  A pointer to a new struct describing the instance
     */
    struct cleafcore*           cleafcore_new();

    /**
     * @brief   Deletes the passed Leafcore instance. DO NOT USE IT afterwards!
     * @param   core            The instance to delete
     */
    void                        cleafcore_delete(struct cleafcore* core);

    /**
     * @brief   Returns the error code of the last Leafcore operation (0 if no error)
     * @param   core            The Leafcore instance to check
     */
    uint16_t                    cleafcore_getError(struct cleafcore* core);

    /**
     * @brief   Returns a string describing the error of the last Leafcore operation ('No error' if no error)
     * @param   core            The Leafcore instance to check
     * @return  char*           The pointer to the string, DELETE IT AFTER USAGE!
     */
    char*                       cleafcore_getErrorString(struct cleafcore* core);

    /**
     * @brief   Clears the last error of the Leafcore object
     * @param   core            The instance to process
     */
    void                        cleafcore_clear_error(struct cleafcore* core);

    /**
     * @brief   Calls Leafcore::a_update()
     * @param   core            The core instance to use
     * @return  int8_t          CLEAFCORE_OK, else an other error code
     */
    int8_t                      cleafcore_a_update(struct cleafcore* core);

    /**
     * @brief   Calls Leafcore::a_install()
     * @param   core            The core instance to use
     * @param   len_pkgs        The amount of packages to install
     * @param   pkgs            An array of const char* strings with the package names
     * @return  int8_t          CLEAFCORE_OK, else an other error code
     */
    int8_t                      cleafcore_a_install(struct cleafcore* core, uint32_t len_pkgs, const char* pkgs[]);

    /**
     * @brief   Calls Leafcore::a_installLocal()
     * @param   core            The core instance to use
     * @param   len_pkgs        The amount of packages to install
     * @param   pkgs            An array of const char* strings with the package paths
     * @return  int8_t          CLEAFCORE_OK, else an other error code
     */
    int8_t                      cleafcore_a_installLocal(struct cleafcore* core, uint32_t len_pkgs, const char* pkgs[]);

    /**
     * @brief   Calls Leafcore::a_upgrade()
     * @param   core            The core instance to use
     * @param   len_pkgs        The amount of packages to process
     * @param   pkgs            An array of const char* strings with the package names
     * @return  int8_t          CLEAFCORE_OK, else an other error code
     */
    int8_t                      cleafcore_a_upgrade(struct cleafcore* core, uint32_t len_pkgs, const char* pkgs[]);

    /**
     * @brief   Calls Leafcore::a_remove()
     * @param   core            The core instance to use
     * @param   len_pkgs        The amount of packages to process
     * @param   pkgs            An array of const char* strings with the package names
     * @return  int8_t          CLEAFCORE_OK, else an other error code
     */
    int8_t                      cleafcore_a_remove(struct cleafcore* core, uint32_t len_pkgs, const char* pkgs[]);

#ifdef __cplusplus
}
#endif

#endif
