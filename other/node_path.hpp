#ifndef _NODE_PATH_H
#define _NODE_PATH_H


#include <vector>
#include <string>
#include "rapidxml.hpp"


class node_path
{
    private:
    /** The path elements, i.e. the single node names of the path. */
    std::vector<std::string> elements_;


    public:
    /** \brief Constructs a node path without elements. */
    node_path() {};

    /** \brief Constructs a node path from a string. */
    node_path(const std::string & path);

    const std::vector < std::string > & elements() const
        { return elements_; }

    /** \brief Searches for the node corresponding to the path inside an XML-document. */
    const rapidxml::xml_node<> * find_node(const rapidxml::xml_document<char> & root) const;

    /** \brief Gets the value of the node corresponding to the path inside an XML-document. */
    const std::string node_value(const rapidxml::xml_document<char> & root) const;

    /** \brief Counts the siblings of the node corresponding to the path. */
    int count_siblings(const rapidxml::xml_document<char> & root) const;

    /** \brief Gets the string representation of the node path. */
    std::string path_string() const;


    private:
    static constexpr char separator_ = '/';

    /** \brief Counts the elements of a node-path string. */
    static int count_path_elements(const std::string & path);

    /** \brief Copies the elements of \a path into the container iterated over
     *  by \a begin_element and \a end_element.
     */
    static void extract_path_elements(std::vector<std::string>::iterator begin_element, const std::vector<std::string>::iterator & end_element, const std::string & path);

};

#endif
