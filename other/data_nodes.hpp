#ifndef _DATA_NODES_H
#define _DATA_NODES_H


#include <memory>

#include "node_path.hpp"
#include "xml_file.hpp"

class xml_file;

/**
 *
 *  \warning The pointers returned by \a begin() and \a end() may be invalidated when file_ is updated via new_file() from the outside.
 */
class data_nodes
{
    private:
    node_path path_;

    std::shared_ptr<xml_file> file_;

    int num_nodes_;


    public:
    const std::string name;

    data_nodes(const node_path & data_npath, const std::shared_ptr<xml_file> & file)
    : path_(data_npath), file_(file), num_nodes_(data_npath.count_siblings(*file)), name(data_npath.elements().back()) {}

    int num_nodes() const { return num_nodes_; }

    const rapidxml::xml_node<> * begin() const { return path_.find_node(*file_); }

    const rapidxml::xml_node<> * end() const { return nullptr; }

};

#endif
