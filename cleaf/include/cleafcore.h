/**
 * @file        cleafcore.h
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       A C wrapper for the Leafcore class and its functions
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#ifndef __CLEAFCORE_H__
#define __CLEAFCORE_H__

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
     * @return  const char*     The pointer to the string, DELETE IT AFTER USAGE!
     */
    const char*                 cleafcore_getErrorString(struct cleafcore* core);

    /**
     * @brief   Clears the last error of the Leafcore object
     * @param   core            The instance to process
     */
    void                        cleafcore_clear_error(struct cleafcore* core);

#ifdef __cplusplus
}
#endif

#endif
