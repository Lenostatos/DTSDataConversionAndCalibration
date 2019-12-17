#ifndef MY_EXCEPTIONS_FILE_ERROR_H
#define MY_EXCEPTIONS_FILE_ERROR_H

/** \brief  Contains custom exceptions.
 *
 *  All exceptions are derived from std::exception.
 */
namespace my_exceptions {

/// Intended for problems with files.
class file_error : public std::exception
{
    public:
    virtual const char * what() const noexcept
        { return "There was a problem with a file."; }
};

} // namespace my_exceptions

#endif
