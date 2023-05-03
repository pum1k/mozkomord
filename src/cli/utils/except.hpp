#ifndef CLI_UTILS_EXCEPT_HPP_
#define CLI_UTILS_EXCEPT_HPP_

#include <exception>
#include <string>

namespace utils
{

/**
 * Base class for exceptions caused by program syntax error.
 */
class syntax_error : public std::exception
{
 public:
    virtual const char *what() const noexcept override
    {
        return "Syntax error";
    };
};

/**
 * Exception signaling unexpected token in program source code.
 * This class holds line and column number of the place where the error
 * occurred.
 */
class unexpected_token_error : public syntax_error
{
 private:
    std::pair<int, int> pos;
    std::string msg;
    void init_msg();

 public:
    unexpected_token_error() : pos({0, 0}) { this->init_msg(); };
    unexpected_token_error(int line, int col) : pos({line, col})
    {
        this->init_msg();
    };
    unexpected_token_error(const std::pair<int, int> &pos) : pos(pos){};

    virtual const char *what() const noexcept override;

    bool has_pos() const noexcept;
    const std::pair<int, int> &get_pos() const noexcept { return pos; };
};

/**
 * Exception signaling missing token.
 * Does not hold any additional data about the error.
 */
class missing_token_error : public syntax_error
{
 private:
    std::string msg;

 public:
    missing_token_error(std::string token);
    virtual const char *what() const noexcept override;
};

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
