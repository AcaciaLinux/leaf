#ifndef __FAIL_H__
#define __FAIL_H__

#include <string>

bool leaf_fail(std::string function, std::string message);

#define FAIL(message) leaf_fail(__PRETTY_FUNCTION__, message)

#endif