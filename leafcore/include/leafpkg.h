#ifndef __LEAFPKG_PARSER_H__
#define __LEAFPKG_PARSER_H__

#include <string>
#include <istream>
#include <deque>

typedef struct leafpkg_s{
	std::string name;
	std::string version;
	uint32_t real_version;
	std::string description;
	std::deque<std::string> dependencies;
} leafpkg_t;

namespace LeafPkg{
	/**
	 * @brief	Parses the leaf.pkg stream
	 * @param	in			The input stream to parse
	 * @return	leafpkg_t	The parsed leaf.pkg
	 */
	leafpkg_t				parse(std::istream& in);
}

#endif