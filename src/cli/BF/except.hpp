/**
 * This file defines exceptions used by other BF classes/functions.
 * All of these exceptions inherit (possibly indirectly) from std::exception.
 */

#ifndef CLI_BF_EXCEPT_HPP_
#define CLI_BF_EXCEPT_HPP_

#include <exception>
#include <string>
#include <utility>

#include "strings.hpp"

namespace BF
{

/**
 * Base class for exceptions caused by BF syntax error.
 */
class syntax_error : public std::exception
{
 public:
    virtual const char *what() const noexcept override
    {
        return "BF::syntax_error";
    };
};

/**
 * Exception signaling unexpected closing bracket in BF source code.
 * This class holds line and column number of the place where the error
 * occurred.
 */
class unexpected_bracket_error : public syntax_error
{
 private:
    std::pair<int, int> pos;
    std::string msg;
    void init_msg();

 public:
    unexpected_bracket_error() : pos({0, 0}) { this->init_msg(); };
    unexpected_bracket_error(int line, int col) : pos({line, col})
    {
        this->init_msg();
    };
    unexpected_bracket_error(const std::pair<int, int> &pos) : pos(pos){};

    virtual const char *what() const noexcept override;

    bool has_pos() const noexcept;
    const std::pair<int, int> &get_pos() const noexcept { return pos; };
};

/**
 * Exception signaling missing closing bracket.
 * Does not hold any additional data about the error.
 */
class missing_bracket_error : public syntax_error
{
 private:
    std::string msg;

 public:
    missing_bracket_error();
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

/**
 * This exception will be thrown by BF memory factory when the requested type is
 * not valid.
 */
class mem_type_error : public std::bad_alloc
{
 private:
 public:
    virtual const char *what() const noexcept override;
};

/**
 * This exception will be thrown by BF interpreter factory when the requested
 * type is not valid.
 */
class inter_type_error : public std::bad_alloc
{
 private:
 public:
    virtual const char *what() const noexcept override;
};

} // namespace BF

#endif // CLI_BF_EXCEPT_HPP_
