#ifndef CLI_UTILS_EXCEPT_HPP_
#define CLI_UTILS_EXCEPT_HPP_

#include <exception>
#include <string>

namespace utils
{

/**
 * Exception signaling error while trying to open file.
 */
class load_file_error : public std::exception
{
 private:
    std::string msg;

 public:
    load_file_error(const std::string &filename);
    virtual const char *what() const noexcept override;
};

} // namespace utils

#endif // CLI_UTILS_EXCEPT_HPP_
