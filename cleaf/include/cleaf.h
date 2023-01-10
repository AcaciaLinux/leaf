/**
 * @file		cleaf.h
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The header containing the boilerplate code for using cleaf proerly
 * @copyright	Copyright (c) 2022
 */

#ifndef __CLEAF_H__
#define __CLEAF_H__

#include "loglevels.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief   Reports the major version of leafcore
     */
    uint32_t                    cleaf_get_leafcore_major();

    /**
     * @brief   Reports the minor version of leafcore
     */
    uint32_t                    cleaf_get_leafcore_minor();

    /**
     * @brief   Reports the patch version of leafcore
     */
    uint32_t                    cleaf_get_leafcore_patch();

    /**
     * @brie    Initializes the cleaf runtime with some leaf dependencies such as the Log module
     * @param   level           The loglevel to use
     */
    void                        cleaf_init(enum loglevel level);

    /**
     * @brief   Cleanly finalizes the cleaf wrapper
     * @warning Calling ANY function provided by this library after this will result in undefined behaviour!
     */
    void                        cleaf_finalize();

    /**
     * @brief   Returns if cleaf is initialized and ready
     * @return  uint8_t         == 0 -> false, == 1 (!= 0) -> true
     */
    uint8_t                     cleaf_is_initialized();

    /**
     * @brief   Returns if cleaf owns the log instance and will delete it on cleaf_finalize()
     * @return  uint8_t         == 0 -> false, == 1 (!= 0) -> true
     */
    uint8_t                     cleaf_owns_log();

    /**
     * @brief   Applies the supplied loglevel to the Log module
     * @param   level           The loglevel to use
     */
    void                        cleaf_setLogLevel(enum loglevel level);

    /**
     * @brief   Returns a const char* to the leaf log, do NOT delete this pointer!
     */
    const char*                 cleaf_get_log();

    /**
     * @brief   Clears the internal log stringstream
     */
    void                        cleaf_clear_log();

#ifdef __cplusplus
}
#endif

#endif