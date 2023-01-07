#ifndef __DIST_H__
#define __DIST_H__

#define LEAFCORE_V_MAJOR 0
#define LEAFCORE_V_MINOR 4
#define LEAFCORE_V_PATCH 0

#define LEAFCORE_CODENAME "Sugar Maple"

#define STR_(X) #X
#define STR(X) STR_(X)

#define LEAFCORE_VERSION STR(LEAFCORE_V_MAJOR) "." STR(LEAFCORE_V_MINOR) "." STR(LEAFCORE_V_PATCH)

#endif
