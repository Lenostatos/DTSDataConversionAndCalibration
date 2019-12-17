
#include "xml_file.hpp"

xml_file::xml_file(std::string && file)
: file_(file)
{
  // Bouml preserved body begin 0002198D
    this->parse<rapidxml::parse_non_destructive | rapidxml::parse_trim_whitespace>(const_cast<char *>(file_.c_str()));
    // TODO: check whether parse_fastest is possible here
    // Note: I'm pretty sure it isn't because I think I need the construction of data nodes...
  // Bouml preserved body end 0002198D

}

void xml_file::new_file(std::string && file)
{
  // Bouml preserved body begin 00021A0D
    this->clear();
    // TODO: test whether this speeds up or slows down the process or maybe is necessary to free up occupied memory

    file_ = file;

    this->parse<rapidxml::parse_non_destructive | rapidxml::parse_trim_whitespace>(const_cast<char *>(file_.c_str()));
    // TODO: check whether parse_fastest is possible here
    // Note: I'm pretty sure it isn't because I think I need the construction of data nodes...
  // Bouml preserved body end 00021A0D

}

