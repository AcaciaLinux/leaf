#include "log.h"
#include "leafdebug.h"

#include <string>
#include <map>

std::map<std::string, bool> function_fail;

bool leaf_debug_check_function_fail(std::string funName){
	if (function_fail[funName]){
		LOGW("Function " + funName + " will fail for debugging now");
		return true;
	}
	return false;
}

void leaf_debug_set_function_fail(std::string funName, bool state){
	if (state)
		LOGW("Setting up function " + funName + " for debugging failure");
	else
		LOGW("Tearing down function " + funName + " debug failure");

	function_fail[funName] = state;
}

#ifdef DEBUG

	LeafDebugGuard::LeafDebugGuard(std::string name){
		_funName = name;
		leaf_debug_set_function_fail(_funName, true);
	}

	LeafDebugGuard::~LeafDebugGuard(){
		leaf_debug_set_function_fail(_funName, false);
	}

#endif
