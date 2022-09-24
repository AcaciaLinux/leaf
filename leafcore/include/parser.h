#ifndef __PARSER_H__
#define __PARSER_H__

#include <istream>
#include <map>
#include <string>

class Parser{

public:
    
    /**
     * @brief   Parses the supplied stream
     * @param   inStream                The stream to parse from
     */
    void                                parse(std::istream& inStream);

    /**
     * @brief   Gets the entry of the supplied key, default if not found
     * @param   key                     The key to search for
     * @param   default                 The default value to return if the key is not existing
     * @return  std::string             The value of the key
     */
    std::string                         get(const std::string& key, const std::string& def = "");

private:
    std::map<std::string, std::string>  _entries;

};

#endif
