#ifndef CLI_BF_EXCEPT_HPP_
#define CLI_BF_EXCEPT_HPP_

#include <exception>
#include <string>
#include <utility>

namespace BF
{
class syntax_error : public std::exception
{
 public:
    virtual const char *what() const noexcept override
    {
        return "BF::syntax_error";
    };
};

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

class missing_bracket_error : public syntax_error
{
 public:
    virtual const char *what() const noexcept override;
};

class load_file_error : public std::exception
{
 private:
    std::string msg;

 public:
    load_file_error(const std::string &filename);
    virtual const char *what() const noexcept override;
};

} // namespace BF

#endif // CLI_BF_EXCEPT_HPP_
