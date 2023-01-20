#ifndef __CLEAF_LOGLEVELS_H__
#define __CLEAF_LOGLEVELS_H__

enum loglevel{
    LOGLEVEL_UE,                //User Error
    LOGLEVEL_UW,                //User Warning
    LOGLEVEL_U,                 //User
    LOGLEVEL_E,                 //Error
    LOGLEVEL_W,                 //Warning
    LOGLEVEL_I,                 //Info
    LOGLEVEL_P,                 //Programmer info (implementation warnings...)
    LOGLEVEL_DE,                //Debug Error (for developer)
    LOGLEVEL_DW,                //Debug Warning (for developer)
    LOGLEVEL_DI,                //Debug Info (for developer)
    LOGLEVEL_D,                 //Debug
    LOGLEVEL_API,               //API calls / requests
    LOGLEVEL_F,                 //Files
    LOGLEVEL_N,                 //Networking
    LOGLEVEL_IO,                //Input / Output (Bus)
    LOGLEVEL_MEM,               //Memory (allocating...)
    LOGLEVEL_FUNCALLS,          //Function calls being plot
    LOGLEVEL_A                  //Everything
};

#endif
