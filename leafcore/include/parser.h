#ifndef __PARSER_H__
#define __PARSER_H__

#include <istream>
#include <map>
#include <string>

class Parser{

public:
    
    void                                parse(std::istream& inStream);

private:
    std::map<std::string, std::string>  _entries;

};

#endif
