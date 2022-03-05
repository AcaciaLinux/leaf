/**
 * @file		leafdebug.h
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		A interface into laef for debugging
 * 				This header contains lots of switches that can make a function
 * 				fail to test the behaviour of leaf if certain parts break
 * @copyright 	Copyright (c) 2022
 */

#ifndef __LEAF_DEBUG_H__
#define __LEAF_DEBUG_H__

#include <vector>
#include <string>

#ifdef DEBUG

	class LeafDebugGuard{
	
	public:
		LeafDebugGuard(std::string funName);
		~LeafDebugGuard();

	private:
		std::string		_funName;
	};

	bool leaf_debug_check_function_fail(std::string funName);
	void leaf_debug_set_function_fail(std::string funName, bool state);

	#define LEAF_DEBUG(funName) if(leaf_debug_check_function_fail(funName)) return false
	#define LEAF_DEBUG_SET_FAIL(funName) LeafDebugGuard functionDebugGuard(funName)
#else
	#define LEAF_DEBUG(funName)
	#define LEAF_DEBUG_SET_FAIL(funName)
#endif

#endif