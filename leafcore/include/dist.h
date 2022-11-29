#ifndef __DIST_H__
#define __DIST_H__

#define LEAFCORE_V_MAYOR 0
#define LEAFCORE_V_MINOR 3
#define LEAFCORE_V_PATCH 0

#define LEAFCORE_CODENAME "Box Elder"

#define STR_(X) #X
#define STR(X) STR_(X)

#define LEAFCORE_VERSION STR(LEAFCORE_V_MAYOR) "." STR(LEAFCORE_V_MINOR) "." STR(LEAFCORE_V_PATCH)

#endif
