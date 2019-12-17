#ifndef _XML_FILE_H
#define _XML_FILE_H


#include <string>

#include "rapidxml.hpp"

/** \brief  Extends the xml_document<char> class with a private string for holding a file.
 *
 *  It is ensured that the file-string persists until a new file is parsed into the
 *  xml_document object. This is necessary because the xml_document only holds pointers
 *  to the file-string.
 */
class xml_file : public rapidxml::xml_document<char>
{
    private:
    std::string file_;


    public:
    explicit xml_file(std::string && file);

    xml_file (xml_file && other) = default;

    void new_file(std::string && file);

};

#endif
