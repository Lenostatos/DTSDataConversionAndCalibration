#ifndef MY_EXCEPTIONS_TAG_ERROR_H
#define MY_EXCEPTIONS_TAG_ERROR_H

/** \brief  Contains custom exceptions.
 *
 *  All exceptions are derived from std::exception.
 */
namespace my_exceptions {

/// Intended for problems with finding tags or their content in the configuration file.
class tag_error : public std::exception
{
    public:
    virtual const char * what() const noexcept
        { return "There was a problem with the content of the configuration file."; }
};

} // namespace my_exceptions

#endif
