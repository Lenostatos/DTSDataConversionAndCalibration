#ifndef OUTPUT_DIRECTORY_H
#define OUTPUT_DIRECTORY_H


#include <string>

namespace input { class configuration; }

namespace output {

/// Represents the directory that the output is supposed to be written to.
class directory
{
    private:
    std::string path_;


    public:
    directory(const input::configuration & config);

    const std::string & path() const { return path_; }

};

} // namespace output

#endif
