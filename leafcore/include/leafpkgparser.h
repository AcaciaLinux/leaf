#ifndef __LEAFPKG_PARSER_H__
#define __LEAFPKG_PARSER_H__

#include <string>
#include <istream>

/**
 * @brief	This class represents a parser for the leaf.pkg file in a package
 */
class LeafPkgParser{

public:

	/**
	 * @brief	Parse the stream provided
	 * @param	in				The stream to parse the file from
	 */
	void						parse(std::istream& in);

private:

	std::string					_error;

};

#endif