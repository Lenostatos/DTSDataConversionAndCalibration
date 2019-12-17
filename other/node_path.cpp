
#include "node_path.hpp"

#include <iostream>

#include "my_exceptions/node_path_error.hpp"

/**
 *  Interprets the argument as a kind-of X-Path which consists of
 *  a leading slash and trailing node-names separated by slashes.
 *  The node-names are stored in the \a elements_ member in the same
 *  order as they appear in the argument.
 *
 *  If the last character of the argument is a slash, then the last
 *  node-name stored in \a elements_ will be an empty string to indicate
 *  that the first child node at that path is meant, regardless of it's name.
 *
 *  If node names between slashes are omitted, this is also represented as
 *  an empty string in the \a elements_ vector for the same reason as above.
 *
 *  \todo Look up whether XML-tag names can contain whitespaces. (I think not.)
 *  If no, create a function that validates the path string
 *  (i.e. checks for path[0] == separator && !contains_whitespaces(path)).
 *
 *  \param[in]  path    A string containing an absolute Path to an XML-node.
 *
 *  \exception  my_exceptions::node_path_error   Thrown if \a path, doesn't start with a separator.
 */
node_path::node_path(const std::string & path)
{
  // Bouml preserved body begin 0002958D
    if (path.empty()) {} // same as the default constructor without arguments
    else if (path[0] != separator_)
    {
        std::cerr << "\nError: Invalid XML-Path \"" + path + "\" doesn't start with the separator \"" + separator_ + "\"!\n" << std::endl;
        throw my_exceptions::node_path_error();
    }
    else
    {
        // first count the path elements
        int num_elements = count_path_elements(path);

        elements_.resize(num_elements);

        // now copy the elements into the internal data structure
        extract_path_elements(elements_.begin(), elements_.end(), path);
    }
  // Bouml preserved body end 0002958D

}

/**
 *  \param[in]  root    An XML-document object.
 *
 *  \exception  my_exceptions::node_path_error  Thrown if the path contains no elements or doesn't match with \a root.
 *
 *  \return A pointer to the XML-node corresponding to the path.
 */
const rapidxml::xml_node<> * node_path::find_node(const rapidxml::xml_document<char> & root) const
{
  // Bouml preserved body begin 0002960D
    if (elements_.empty())
    {
        std::cerr << "\nError: Tried to find an XML-node with an empty path!\n" << std::endl;
        throw my_exceptions::node_path_error();
    }
    // leave this out for now
//    else if ((elements_[0] != "") && (elements_[0] != std::string(root.name(), root.name_size())))
//    {
//        std::cout << "\nError: Found node \"" << std::string(root.name(), root.name_size())
//                  << "\" instead of node \"" << elements_[0] << "\" from the path \""
//                  << this->path_string() << "\"!\n" << std::endl;
//        throw std::invalid_argument("Error: XML-path doesn't match the passed XML-document!");
//    }
    else
    {
        const rapidxml::xml_node<> * iterator_node = &root;
        // the node behind root seems to be one step above the visible root node in the document
        // (e.g. in the Kloendeel files this visible root node would be "logs",
        // but "logs" seems to be a child node of root)
        // maybe i haven't completely understood the DOM yet...

        auto iterator_elem = begin(elements_);

        // leave this out for now
//        ++iterator_elem; // because the root is already checked

        // iterating towards the wanted node
        while (iterator_node && (iterator_elem != end(elements_)))
        {
            if (iterator_elem->empty())
                { iterator_node = iterator_node->first_node(); }
            else
                { iterator_node = iterator_node->first_node(iterator_elem->c_str()); }
            ++iterator_elem;
        }

        // if the iteration ended too early path and XML-document didn't match
        if (iterator_elem != end(elements_))
        {
            // outputting an additional message because the additional operations could throw an exception
            // inside the throw statement and that could lead to a std::terminate() call!
            std::cerr << "\nError: Found node \"" << std::string(iterator_node->name(), iterator_node->name_size())
                      << "\" instead of node \"" << *iterator_elem << "\" from the path \""
                      << this->path_string() << "\"!\n"
                      << "Error: XML-path doesn't match the passed XML-document!\n"
                      << std::endl;
            throw my_exceptions::node_path_error();
        }
        else if (!iterator_node)
        {
            std::cerr << "\nError: Could not find node \"" << *iterator_elem << "\" from the path \""
                      << this->path_string() << "\"!\n"
                      << "Error: XML-path doesn't match the passed XML-document!\n"
                      << std::endl;
            throw my_exceptions::node_path_error();
        }
        else
            { return iterator_node; }
    }
  // Bouml preserved body end 0002960D
}


const std::string node_path::node_value(const rapidxml::xml_document<char> & root) const
{
    const rapidxml::xml_node<> * node = find_node(root);
    return std::string(node->value(), node->value_size());
}

/**
 *  Counts only the siblings with the same name if the name is specified in the last entry of \a elements_.
 *
 *  \param[in]  root    An XML-document object.
 *
 *  \exception  my_exceptions::node_path_error  Thrown if node can't be found in \a root.
 *
 *  \return The number of siblings of the node corresponding to the path.
 *          If the path contains no elements, 0 is returned.
 *          If the path doesn't match the XML-document a *std::exception* of type *invalid_argument* is thrown.
 */
int node_path::count_siblings(const rapidxml::xml_document<char> & root) const
{
  // Bouml preserved body begin 0002968D
    const rapidxml::xml_node<> * node = find_node(root);

    if (node)
    {
        int num_data_nodes = 0;
        if (elements_.back().empty())
        {
            while (node)
            {
                node = node->next_sibling();
                ++num_data_nodes;
            }
        }
        else
        {
            const char * search_term = elements_.back().c_str();
            while (node)
            {
                node = node->next_sibling(search_term);
                ++num_data_nodes;
            }
        }
        return num_data_nodes;
    }
    else
        { return 0; }
  // Bouml preserved body end 0002968D

}

/**
 *  \return A string containing the path.
 *          If member \a elements_ is empty, an empty string is returned.
 */
std::string node_path::path_string() const
{
  // Bouml preserved body begin 0002970D
    std::string path_string("");

    if (!elements_.empty()){
        for (const std::string & elem : elements_)
            { path_string += "/" + elem; }
    }

    return path_string;
  // Bouml preserved body end 0002970D

}

/**
 *  Effectively counts the number of slashes in the path.
 *
 *  \param[in]  path    A string containing a path.
 *
 *  \return The number of named and nameless elements in the path
 *          If \a path is empty or invalid it causes undefined behavior.
 */
int node_path::count_path_elements(const std::string & path)
{
  // Bouml preserved body begin 0002978D
    size_t pos_fc = 0; // position of first character after most recently found slash
    int num_elements = 0;

    do
    {
        ++num_elements;
        ++pos_fc;
        pos_fc = path.find_first_of(separator_, pos_fc);
    } while (pos_fc != std::string::npos);

    return num_elements;
  // Bouml preserved body end 0002978D
}

/**
 *  \warning Expects path and container to have the same number of elements.
 *  \warning If \a path is empty or invalid it causes undefined behavior.
 *
 *  \todo Throw node_path_error exception if path cannot be interpreted!
 *
 *  \param[in]  begin_element   Iterator to beginning of desired container.
 *  \param[in]  end_element     Iterator to end of desired container.
 *  \param[in]  path            String containing the path.
 */
void node_path::extract_path_elements(std::vector<std::string>::iterator begin_element, const std::vector<std::string>::iterator & end_element, const std::string & path)
{
  // Bouml preserved body begin 0002980D
    size_t pos_fc = 1; // position of first character after most recently found slash
    size_t pos_ns = path.find_first_of(separator_, pos_fc); // position of most recently found slash

    while (begin_element != end_element)
    {
        *begin_element = path.substr(pos_fc, pos_ns - pos_fc);
        ++begin_element;

        pos_fc = pos_ns + 1;
        pos_ns = path.find_first_of(separator_, pos_fc);
    }
  // Bouml preserved body end 0002980D
}

