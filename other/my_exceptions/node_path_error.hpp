#ifndef MY_EXCEPTIONS_NODE_PATH_ERROR_H
#define MY_EXCEPTIONS_NODE_PATH_ERROR_H

/** \brief  Contains custom exceptions.
 *
 *  All exceptions are derived from std::exception.
 */
namespace my_exceptions {

/// Intended for problems with *node_path* objects.
class node_path_error : public std::exception
{
    public:
    virtual const char * what() const noexcept
        { return "There was a problem with an XML-Path."; }
};

} // namespace my_exceptions

#endif
